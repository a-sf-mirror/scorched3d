////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////

#include <server/ServerAdminHandler.h>
#include <server/ScorchedServer.h>
#include <server/ScorchedServerUtil.h>
#include <server/ServerChannelManager.h>
#include <server/ServerCommon.h>
#include <server/ServerAdminCommon.h>
#include <server/ServerAdminSessions.h>
#include <server/ServerMessageHandler.h>
#include <common/StatsLogger.h>
#include <common/Logger.h>
#include <common/Defines.h>
#include <coms/ComsAdminMessage.h>
#include <coms/ComsAdminResultMessage.h>
#include <coms/ComsSyncCheckMessage.h>
#include <coms/ComsMessageSender.h>
#include <net/NetInterface.h>
#include <tank/TankContainer.h>
#include <tank/TankState.h>
#include <stdlib.h>

ServerAdminHandler *ServerAdminHandler::instance()
{
	static ServerAdminHandler *instance = 
		new ServerAdminHandler;
	return instance;
}

ServerAdminHandler::ServerAdminHandler()
{
	ScorchedServer::instance()->getComsMessageHandler().addHandler(
		"ComsAdminMessage",
		this);
}

ServerAdminHandler::~ServerAdminHandler()
{
}

bool ServerAdminHandler::processMessage(
	NetMessage &netMessage,
	const char *messageType,
	NetBufferReader &reader)
{
	ComsAdminMessage message;
	if (!message.readMessage(reader)) return false;

	unsigned int destinationId = netMessage.getDestinationId();

	ServerMessageHandler::DestinationInfo *destinationInfo =
		ServerMessageHandler::instance()->getDestinationInfo(destinationId);
	if (!destinationInfo) return false;

	// Login if that is what is happening
	if (message.getType() == ComsAdminMessage::AdminLogin ||
		message.getType() == ComsAdminMessage::AdminLoginLocal)
	{
		unsigned int sid = ServerAdminSessions::instance()->
			login(message.getParam1(), message.getParam2(),
			NetInterface::getIpName(netMessage.getIpAddress()));
		if (sid != 0)
		{
			ServerAdminSessions::SessionParams *adminSession =
				ServerAdminSessions::instance()->getSession(sid);

			ServerChannelManager::instance()->refreshDestination(destinationId);

			ServerCommon::sendString(0,
				S3D::formatStringBuffer("Server admin \"%s\" logged in",
				adminSession->credentials.username.c_str()));
			ServerCommon::serverLog(
				S3D::formatStringBuffer("Server admin \"%s\" logged in, destination id \"%u\"",
				adminSession->credentials.username.c_str(),
				destinationId));

			ComsAdminResultMessage resultMessage(sid);
			ComsMessageSender::sendToSingleClient(resultMessage, destinationId);
			destinationInfo->admin = true;
			destinationInfo->adminTries = 0;
		}
		else
		{
			if (message.getType() != ComsAdminMessage::AdminLoginLocal)
			{
				destinationInfo->adminTries++;
				
				ServerCommon::sendString(destinationId,
					S3D::formatStringBuffer("Incorrect admin password (try %i/3)", 
					destinationInfo->adminTries));
				if (destinationInfo->adminTries > 3)
				{
					ServerCommon::kickDestination(destinationId);
				}

				Logger::log(S3D::formatStringBuffer(
					"Failed login for server admin \"%s\" via console, ip \"%s\", destination id \"%u\"",
					message.getParam1(),
					NetInterface::getIpName(netMessage.getIpAddress()),
					destinationId));
			}

			ComsAdminResultMessage resultMessage(0);
			ComsMessageSender::sendToSingleClient(resultMessage, destinationId);
			destinationInfo->admin = false;
		}
		return true;
	}

	// Only allow logged in tanks (may have timed out)
	ServerAdminSessions::SessionParams *adminSession =
		ServerAdminSessions::instance()->getSession(message.getSid());
	if (!adminSession)
	{
		ServerCommon::sendString(destinationId,
			"You are not logged in as admin");

		ComsAdminResultMessage resultMessage(0);
		ComsMessageSender::sendToSingleClient(resultMessage, destinationId);
		destinationInfo->admin = false;

		return true;		
	}
	const char *adminName = adminSession->credentials.username.c_str();

	// Do admin fn (we are logged in at this point)
	switch (message.getType())
	{
	case ComsAdminMessage::AdminShow:
		{
			std::map<unsigned int, Tank *> &tanks = 
				ScorchedServer::instance()->getTankContainer().getAllTanks();
			std::string result;
			result += 
				"--Admin Show-----------------------------------------\n";
			std::map<unsigned int, Tank *>::iterator itor;
			for (itor = tanks.begin();
				itor != tanks.end();
				itor++)
			{
				Tank *tank = (*itor).second;

				result += 
					S3D::formatStringBuffer("%i \"%s\" \"%s\" \"%u\" %s \n",
						tank->getPlayerId(), 
						tank->getName(),
						NetInterface::getIpName(tank->getIpAddress()),
						StatsLogger::instance()->getStatsId(tank->getUniqueId()),
						(tank->getState().getMuted()?"Muted":"Not Muted"));
			}
			result +=
				"-----------------------------------------------------\n";

			ServerCommon::sendString(destinationId, result.c_str());
		}
		break;
	case ComsAdminMessage::AdminLogout:
		{
			ServerCommon::sendString(0,
				S3D::formatStringBuffer("Server admin \"%s\" logged out",
				adminName));
			ServerCommon::serverLog(
				S3D::formatStringBuffer("Server admin \"%s\" logged out",
				adminName));

			ServerAdminSessions::instance()->logout(message.getSid());
			ServerChannelManager::instance()->refreshDestination(destinationId);

			ComsAdminResultMessage resultMessage(0);
			ComsMessageSender::sendToSingleClient(resultMessage, destinationId);
			destinationInfo->admin = false;
		}
		break;
	case ComsAdminMessage::AdminShowBanned:
		{
			std::string result;
			result += 
				"--Admin Show Banned----------------------------------\n";

			std::list<ServerBanned::BannedRange> &bannedIps = 
				ScorchedServerUtil::instance()->bannedPlayers.getBannedIps();
			std::list<ServerBanned::BannedRange>::iterator itor;
			for (itor = bannedIps.begin();
				itor != bannedIps.end();
				itor++)
			{
				ServerBanned::BannedRange &range = (*itor);
				std::string mask = NetInterface::getIpName(range.mask);

				std::map<unsigned int, ServerBanned::BannedEntry>::iterator ipitor;
				for (ipitor = range.ips.begin();
					ipitor != range.ips.end();
					ipitor++)
				{
					unsigned int ip = (*ipitor).first;
					ServerBanned::BannedEntry &entry = (*ipitor).second;
					std::string ipName = NetInterface::getIpName(ip);

					result += S3D::formatStringBuffer("\"%s:%s:%s\" %s %s (%s) - %s",
						entry.name.c_str(),
						entry.uniqueid.c_str(),
						entry.SUI.c_str(),
						ServerBanned::getBannedTypeStr(entry.type),
						ipName.c_str(), mask.c_str(),
						(entry.bantime?ctime(&entry.bantime):"\n"));
				}
			}
			result +=
				"-----------------------------------------------------\n";

			ServerCommon::sendString(destinationId, result.c_str());
		}
		break;
	case ComsAdminMessage::AdminBan:
		{
			if (!ServerAdminCommon::banPlayer(
				adminSession->credentials,
				atoi(message.getParam1()), "<via console>"))
			{
				ServerCommon::sendString(destinationId, "Unknown player for ban");
			}
		}
		break;
	case ComsAdminMessage::AdminFlag:
		{
			if (!ServerAdminCommon::flagPlayer(
				adminSession->credentials,
				atoi(message.getParam1()), "<via console>"))
			{
				ServerCommon::sendString(destinationId, "Unknown player for flag");
			}
		}
		break;
	case ComsAdminMessage::AdminPoor:
		{
			if (!ServerAdminCommon::poorPlayer(
				adminSession->credentials,
				atoi(message.getParam1())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for poor");
			}
		}
		break;	
	case ComsAdminMessage::AdminKick:
		{
			if (!ServerAdminCommon::kickPlayer(
				adminSession->credentials,
				atoi(message.getParam1())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for kick");
			}
		}
		break;
	case ComsAdminMessage::AdminMute:
	case ComsAdminMessage::AdminUnMute:
		{
			bool mute = (message.getType() == ComsAdminMessage::AdminMute);
			if (!ServerAdminCommon::mutePlayer(
				adminSession->credentials,
				atoi(message.getParam1()), mute))
			{
				ServerCommon::sendString(destinationId, "Unknown player for mute");
			}	
		}
		break;
	case ComsAdminMessage::AdminPermMute:
		{
			if (!ServerAdminCommon::permMutePlayer(
				adminSession->credentials,
				atoi(message.getParam1()), "<via console>"))
			{
				ServerCommon::sendString(destinationId, "Unknown player for permmute");
			}	
		}
		break;
	case ComsAdminMessage::AdminUnPermMute:
		{
			if (!ServerAdminCommon::unpermMutePlayer(
				adminSession->credentials,
				atoi(message.getParam1())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for inpermmute");
			}
		}
		break;
	case ComsAdminMessage::AdminTalk:
		ServerAdminCommon::adminSay(adminSession->credentials, "info", message.getParam1());
		break;
	case ComsAdminMessage::AdminAdminTalk:
		ServerAdminCommon::adminSay(adminSession->credentials, "admin", message.getParam1());
		break;
	case ComsAdminMessage::AdminMessage:
		ServerAdminCommon::adminSay(adminSession->credentials, "banner", message.getParam1());
		break;
	case ComsAdminMessage::AdminSyncCheck:
		{
			ServerCommon::sendString(destinationId, "sending sync...");
			ComsSyncCheckMessage syncCheck;
			ComsMessageSender::sendToSingleClient(syncCheck, destinationId);
		}
		break;
	case ComsAdminMessage::AdminKillAll:
		ServerAdminCommon::killAll(adminSession->credentials);
		break;
	case ComsAdminMessage::AdminNewGame:
		ServerAdminCommon::newGame(adminSession->credentials);
		break;	
	case ComsAdminMessage::AdminSlap:
		{
			if (!ServerAdminCommon::slapPlayer(
				adminSession->credentials,
				atoi(message.getParam1()), (float) atof(message.getParam2())))
			{
				ServerCommon::sendString(destinationId, "Unknown player for slap");
			}
		}
		break;
	}

	return true;
}
