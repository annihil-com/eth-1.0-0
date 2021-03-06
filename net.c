// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

/*
==============================
All network code
==============================
*/

#define _GNU_SOURCE
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "eth.h"
#include "net.h"

socket_t serverConnect(server_t *server, int socketType) {
    // Don't use sockets while watching demos
    if (eth.demoPlayback)
	return -1;

    socket_t sock;
    struct sockaddr_in sin;
    struct hostent *he;

    // Check hostname
    if ((he = gethostbyname(server->hostname)) == NULL) {
    	ethLog("irc: gethostbyname error");
        herror(server->hostname);
        return -1;
    }

    // Create socket
    sock = socket(AF_INET, socketType, 0);
    bcopy(he->h_addr, (char *)&sin.sin_addr, he->h_length);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server->port);

    // Connect the socket
    int result = connect(sock, (struct sockaddr*)&sin, sizeof(sin));
    if (result != 0) {
    	ethLog("net: connect error to %s", server->hostname);
    	perror("eth: connect error");
        return -1;
    }

    // Set non-blocking socket
    fcntl(sock, F_SETFL, O_NONBLOCK);

	return sock;
}

qboolean netSendRaw(socket_t socket, char *buffer, int size) {
	if (send(socket, buffer, size, 0) < 0) {
		ethLog("net: net send raw error\n");
        return qfalse;
	}

	return qtrue;
}

// Send text with a trailing '\n'
qboolean netSendText(socket_t socket, char *format, ...) {
	char buffer[512];
	char trailing = '\n';
	
	va_list arglist;
	va_start(arglist, format);
		int size = vsnprintf(buffer, sizeof(buffer) - sizeof(trailing), format, arglist);
	va_end(arglist);

	// Sanity check
	if (size > strlen(buffer)) {
		ethLog("error: net send buffer exceed\n");
		return qfalse;
	}

	sprintf(buffer, "%s%c", buffer, trailing);
	
	return netSendRaw(socket, buffer, strlen(buffer));
}


// This function get all servers infos from the master servers
// Return false if error
static server_t servers[MAX_SERVERS];	// All servers received
static int serverCount;				// Numbers of servers
static qboolean serversInit;			// True when request servers done

qboolean getServersList() {
	#define NET_RAW_SERVER_SIZE 7
	static socket_t socket = -1;
	static int requestTime = 0;
	
	// Connect to master server
	if (socket == -1) {
		socket = serverConnect(&NET_ET_MASTER_SERVER, SOCK_DGRAM);
		netSendText(socket, NET_SERVERS_REQUEST);
		requestTime = eth.cg_time;
		return qtrue;
	}

	// If connect error
	if (socket == -1) {
		ethLog("error: can't connect to ET master server\n");
		return qfalse;
	}

	// Parse response from master server
	if ((requestTime + NET_SERVERS_TIMEOUT) > eth.cg_time) {
		char buffer[(MAX_SERVERS * NET_RAW_SERVER_SIZE) + strlen(NET_SERVERS_RESPONSE) + 1];
	    memset(buffer, 0, sizeof(buffer));

	    int size = recv(socket, buffer, sizeof(buffer), 0);

	    // recv() error
		if (size == 0) {
			ethLog("error: getServerList: can't receive data\n");
			close(socket);
			socket = -1;
			return qfalse;
		}
	    
	    // No data received
		if (size == -1)
			return qtrue;

		// Invalid packet received
		if ((strncmp(buffer, NET_SERVERS_RESPONSE, strlen(NET_SERVERS_RESPONSE)) != 0)
				|| (strncmp(&buffer[size - strlen(NET_SERVERS_RESPONSE_END)], NET_SERVERS_RESPONSE_END, strlen(NET_SERVERS_RESPONSE_END)) != 0)) {
			ethLog("error: invalid packet marker from master server\n");
			close(socket);
			socket = -1;
			return qfalse;
		}
		
		int offset = strlen(NET_SERVERS_RESPONSE);

		// Get all servers
		while (offset < (size - strlen(NET_SERVERS_RESPONSE_END))) {
			if (buffer[offset] != '\\') {
				#ifdef ETH_DEBUG
					ethDebug("warning: getServerList: invalid server in packet\n");
				#endif
				return qtrue;
			}

			server_t *server = &servers[serverCount];
			sprintf(server->hostname, "%i.%i.%i.%i", (unsigned char)buffer[offset + 1], (unsigned char)buffer[offset + 2], (unsigned char)buffer[offset + 3], (unsigned char)buffer[offset + 4]);
			server->port = htons(*(uint16_t *)&buffer[offset + 5]);
			serverCount++;
			offset += NET_RAW_SERVER_SIZE;
		}
		return qtrue;
	}

	// Request timeout
	if (serverCount == 0) {
		ethLog("timeout error: can't get servers from ET master server");
		close(socket);
		socket = -1;
		return qfalse;
	}

	// Success !
	#ifdef ETH_DEBUG
		ethDebug("getServerList: received %i servers from ET master server", serverCount);
	#endif
	serversInit = qtrue;
	close(socket);
	socket = -1;
	return qtrue;
}


static char names[MAX_NAMES][MAX_QPATH];
static int nameCount;
static qboolean namesInit;

qboolean getNames(server_t server) {
	static socket_t socket = -1;
	static int requestTime = 0;

	if (socket == -1) {
		server_t *server = &servers[(int)((float)serverCount * rand() / RAND_MAX)];
		socket = serverConnect(server, SOCK_DGRAM);
		netSendText(socket, NET_STATUS_REQUEST);
		requestTime = eth.cg_time;
		return qtrue;
	}

	// If connect error
	if (socket == -1)
		return qtrue;

	// Parse data
	if ((requestTime + NET_STATUS_TIMEOUT) > eth.cg_time) {
		char buffer[8192];
	    memset(buffer, 0, sizeof(buffer));

	    int size = recv(socket, buffer, sizeof(buffer), 0);

	    // recv() error
		if (size == 0) {
			ethLog("error: getNames: can't receive data");
			close(socket);
			socket = -1;
			return qfalse;
		}

	    // No data received
		if (size == -1)
			return qtrue;

		#ifdef ETH_DEBUG
			ethDebug("getNames: received len: %i data: %s", size, buffer);
		#endif

		// Invalid packet received
		if (strncmp(buffer, NET_STATUS_RESPONSE, strlen(NET_STATUS_RESPONSE)) != 0) {
			ethLog("error: getNames: invalid server packet marker");
			close(socket);
			socket = -1;
			return qfalse;
		}

		char *str = buffer;
		str += strlen(NET_STATUS_RESPONSE) + 1;
		str = strchr(str, '\n') + 1;	// Skip server infos
		
		// Check for empty server
		if (!strlen(str))
			return qtrue;

		// Get all names
		while (str && (str < buffer + size) && (nameCount < MAX_NAMES)) {
			char line[128];
			memset(line, 0, sizeof(line));
			strncpy(line, str, strchr(str, '\n') - str);
			
			// Search name
			char *start = strchr(line, '"') + 1;
			char *end = strrchr(line, '"') - 1;

			// Check infos
			if ((start == NULL) || (end == NULL) || (start == end)) {
				ethLog("warning: getNames: invalid player infos received");
				return qtrue;
			}

			// Get name			
			memset(names[nameCount], 0, sizeof(names[0]));
			strncpy(names[nameCount], start, end - start + 1);

			// Don't add bad names
			char *cleanName = strdup(names[nameCount]);
			Q_CleanStr(cleanName);
			if ((strlen(cleanName) > 2) && !strcasestr(cleanName, "bot"))
				nameCount++;

			free(cleanName);
			
			// Next line
			str = strchr(str, '\n') + 1;
		}

		#ifdef ETH_DEBUG
			ethDebug("getNames: total names: %i", nameCount);
		#endif

		return qtrue;
	}
	
	// Retry if we need more names
	if (nameCount < MAX_NAMES) {
		close(socket);
		socket = -1;
		return qtrue;
	}
	
	// Success !
	namesInit = qtrue;
	close(socket);
	socket = -1;
	return qtrue;
}

// Return false if error
qboolean setRandomName() {
	static int lastNameChange;

	// Wait before set a random name
	if ((lastNameChange + (seth.value[VAR_RANDOMNAME_DELAY] * 1000)) > eth.cg_time)
		return qtrue;

	if (seth.value[VAR_RANDOMNAME] == 2) {
		int count1, count2;
		int availableNickIDs[MAX_CLIENTS];
		for (count1 = 0, count2 = 0; count1 < MAX_CLIENTS; count1++) {
			if (eth.clientInfo[count1].infoValid)
				availableNickIDs[count2++] = count1;
		}
		count2--;
		if (count2 > 0)
			strcpy(eth.clientInfo[eth.cg_clientNum].name, eth.clientInfo[availableNickIDs[rand() % count2]].name);
		else
			return qfalse;
	} else if (seth.value[VAR_RANDOMNAME] == 1) {
	
		// Get all servers
		if (!serversInit)
			return getServersList();

		// Get names from random servers
		if (!namesInit)
			return getNames(servers[(int)((float)serverCount * rand() / RAND_MAX)]);

		// Change name
		strcpy(eth.clientInfo[eth.cg_clientNum].name, names[(int)((float)nameCount * rand() / RAND_MAX)]);
	}

	#define CHANGE_NAME_CMD "name \"%s\"\n"
	char *cmd = calloc(1, strlen(CHANGE_NAME_CMD) + MAX_QPATH + 1);
	sprintf(cmd, CHANGE_NAME_CMD, eth.clientInfo[eth.cg_clientNum].name);
	syscall_CG_SendConsoleCommand(cmd);
	free(cmd);

	lastNameChange = eth.cg_time;
	return qtrue;
}

void getSpectators() {
	static socket_t specSocket = -1;
	static int lastRequestTime = 0;
	char buffer[4096];

	// Connect
	if (specSocket == -1) {
		specSocket = serverConnect(&eth.server, SOCK_DGRAM);
		netSendText(specSocket, NET_STATUS_REQUEST);
		lastRequestTime = eth.cg_time;
		return;
	}

	// Only do request each NET_STATUS_TIMEOUT
	if ((lastRequestTime + NET_STATUS_TIMEOUT) <= eth.cg_time) {
		close(specSocket);
		specSocket = -1;
		return;
	}

	memset(buffer, 0, sizeof(buffer));
	int size = recv(specSocket, buffer, sizeof(buffer), 0);
	
	// If no data receive
	if (size == -1)
		return;
		
	// If valid packet
	if (strncmp(buffer, NET_STATUS_RESPONSE, strlen(NET_STATUS_RESPONSE)) != 0) {
		ethLog("warning: getSpectators: invalid packet");
		return;
	}

	char *str = buffer;
	// Skip header
	str += strlen(NET_STATUS_RESPONSE) + 1;
	// Skip server infos
	str = strchr(str, '\n') + 1;
	
	// Reset spec infos
	seth.specCount = 0;
	memset(seth.specNames, 0, sizeof(seth.specNames));
	
	// Get all names
	while (str && (str < buffer + size)) {
		char line[128];
		memset(line, 0, sizeof(line));
		strncpy(line, str, strchr(str, '\n') - str);
			
		char xp[8];
		memset(xp, 0, sizeof(xp));
		strncpy(xp, str, strchr(str, ' ') - str);
			
		if ((eth.cg_snap->ps.stats[STAT_XP] == atoi(xp)) && (atoi(xp) != 0)) {
			char *start = strchr(line, '"') + 1;
			char *end = strrchr(line, '"') - 1;

			if ((start == NULL) || (end == NULL) || (start == end)) {
				ethLog("warning: invalid player infos");
				break;
			}
				
			char name[MAX_QPATH];
			memset(name, 0, sizeof(name));
			strncpy(name, start, end - start + 1);

			int i = 0;
			for (; i < MAX_CLIENTS; i++) {
				if (eth.clientInfo[i].infoValid && !strcmp(eth.clientInfo[i].name, name)
						&& (eth.clientInfo[i].team == TEAM_SPECTATOR)) {
					strcpy(seth.specNames[seth.specCount], name);
					seth.specCount++;
					break;
				}
			}
		}
			
		// Next line
		str = strchr(str, '\n') + 1;
	}
}

/*
==============================
irc stuff
==============================
*/

// irc infos
static server_t ircServer = { "irc.rizon.net", 6667 };
static char *ircChannel = "#eth-online";
static char ircNick[64];

// All irc buddies on the channel
ircBuddy_t ircBuddies[IRC_MAX_BUDDIES];

// Main irc socket
static socket_t ircSocket = -1;

qboolean netSendIrcChannel(char *format, ...) {
	char buffer[128];
	
	va_list arglist;
	va_start(arglist, format);
		int size = vsnprintf(buffer, sizeof(buffer), format, arglist);
	va_end(arglist);

	if (size > strlen(buffer)) {
		ethLog("error: net send irc buffer exceed");
		return qfalse;
	}

	int msgCount = 0;
	msgCount = IRC_MAX_MSGS - 2;
	while (msgCount >= 0) {
		ircMsgs[msgCount + 1].type = ircMsgs[msgCount].type;
		sprintf(ircMsgs[msgCount + 1].nick,"%s",ircMsgs[msgCount].nick);
		sprintf(ircMsgs[msgCount + 1].msg,"%s",ircMsgs[msgCount].msg);
		msgCount--;		
	}
	ircMsgs[0].type = IRC_MSGTYPE_CHAN;
	sprintf(ircMsgs[0].nick,"^n%s",ircNick);
	strcpy(ircMsgs[0].msg, buffer);

	return netSendText(ircSocket, "%s %s :%s", IRC_PRIVMSG, ircChannel, buffer);
}

qboolean netSendIrcQuery(char *nick, char *format, ...) {
	char buffer[128];
	
	va_list arglist;
	va_start(arglist, format);
		int size = vsnprintf(buffer, sizeof(buffer), format, arglist);
	va_end(arglist);

	if (size > strlen(buffer)) {
		ethLog("error: net send query buffer exceed");
		return qfalse;
	}

	int msgCount = 0;
	msgCount = IRC_MAX_MSGS - 2;
	while (msgCount >= 0) {
		ircMsgs[msgCount + 1].type = ircMsgs[msgCount].type;
		sprintf(ircMsgs[msgCount + 1].nick,"%s",ircMsgs[msgCount].nick);
		sprintf(ircMsgs[msgCount + 1].msg,"%s",ircMsgs[msgCount].msg);
		msgCount--;		
	}
	ircMsgs[0].type = IRC_MSGTYPE_QUERY;
	sprintf(ircMsgs[0].nick,"^3->^n%s", nick);
	strcpy(ircMsgs[0].msg, buffer);

	return netSendText(ircSocket, "%s %s :%s", IRC_PRIVMSG, nick, buffer);
}

qboolean netSendIrcStatus() {
	if (!eth.hookLoad)
		return netSendIrcChannel("Not connected");

	char *serverInfo = eth.cgs_gameState->stringData + eth.cgs_gameState->stringOffsets[CS_SERVERINFO];
	char *serverName = Q_CleanStr(eth_Info_ValueForKey(serverInfo, "sv_hostname"));

	char name[MAX_QPATH];
	syscall_CG_Cvar_VariableStringBuffer("name", name, sizeof(name));
	Q_CleanStr(name);
	
	return netSendIrcChannel("!status |%i| [%s] on server [ET %s] [%s %s] |%s:%i| [%s]",
			eth.cg_clientNum, name, sethET->version, eth.mod.name, eth.mod.version, eth.server.hostname, eth.server.port, serverName);
}

qboolean ircConnect() {
	ircSocket = serverConnect(&ircServer, SOCK_STREAM);

	if (ircSocket == -1) {
		gameMessage(qfalse, "irc: can't connect to %s", ircServer.hostname);
		return qfalse;
	}

	// Set irc bot nick
	if (strlen(ircNick) == 0) {
		char *nick = getenv("ETH_IRC_NICK");
		if (!nick)
			nick = getenv("USER");
		
		// Add bot prefix, so others bots can see it
		snprintf(ircNick, sizeof(ircNick), "%s%s", IRC_BOT_PREFIX, nick);
	}

	// irc register
	netSendText(ircSocket, "%s %s", IRC_NICK, ircNick);
	netSendText(ircSocket, "USER %s 0 * :%s", ircNick, ircNick);

	return qtrue;
}

// return -1 if error
int getBuddySlot(char *name) {
	int count = 0;
	for (; count < IRC_MAX_BUDDIES; count++) {
		if (ircBuddies[count].infoValid && !strcmp(ircBuddies[count].nick, name))
			return count;
	}

	return -1;
}

void addBuddy(char *name) {
	int slot = getBuddySlot(name);
	
	// Buddy already exist
	if (slot != -1) {
		ethLog("irc: error: can't add buddy if already exist: %s on slot %i", name, slot);
		return;
	}
	
	// Find a free slot
	int count = 0;
	for (; count < IRC_MAX_BUDDIES; count++) {
		ircBuddy_t *buddy = &ircBuddies[count];

		// Slot free
		if (!buddy->infoValid) {
			memset(buddy, 0, sizeof(ircBuddy_t));
			strncpy(buddy->nick, name, IRC_NICK_SIZE);
			buddy->infoValid = qtrue;
			
			// If myself copy all infos too
			if (!strcmp(name, ircNick)) {
				memcpy(&buddy->server, &eth.server, sizeof(server_t));
				buddy->playerNumber = eth.cg_clientNum;
			}

			else
				addHudOutputMsg(hudinfo, "%s has joined IRC fireteam", name);
			
			#ifdef ETH_DEBUG
				ethDebug("eth: irc: add buddy %s on slot %i", name, count);
			#endif
			return;
		}
	}
	
	// Can't find a free slot
	ethLog("irc: warning: can't find a free slot for buddy: %s (max=%i)", name, IRC_MAX_BUDDIES);
}

void updateBuddy(char *name, char *infos) {
	int slot = getBuddySlot(name);
	
	// Buddy don't exist
	if (slot == -1) {
		ethLog("irc: warning: can't update infos: buddy not found: %s", name);
		return;
	}
	
	// If invalid infos
	if (!infos || !strlen(infos)) {
		ethLog("irc: error: invalid infos for buddy %s", name);
		return;
	}

	ircBuddy_t *buddy = &ircBuddies[slot];
	
	// Parse infos
	char *offset = infos;
	char *str;
	int infosCount = 0;
	while ((str = strsep(&offset, "|"))) {
		// Get player number
		if (infosCount == 1) {
			int playerNumber = atoi(str);
			// Sanity check
			if ((playerNumber < 0) || (playerNumber >= MAX_CLIENTS)) {
				ethLog("irc: error: invalid player number for buddy %s", name);
				return;
			}
			buddy->playerNumber = playerNumber;
		} else if (infosCount == 3) {
			char *port = strchr(str, ':');
			// Sanity check
			if (!port) {
				ethLog("irc: error: invalid server for buddy %s", name);
				return;
			}
			*port = '\0';
			port++;
			
			strncpy(buddy->server.hostname, str, sizeof(buddy->server.hostname));
			buddy->server.port = atoi(port);
		}
			
		infosCount++;
	}

	// Infos incomplete
	if (infosCount < 3) {
		ethLog("irc: error: can't get all infos for buddy %s", name);
		return;
	}
	
	#ifdef ETH_DEBUG
		ethDebug("irc: infos update for buddy %s player_number=%i server_ip=%s server_port=%i",
				name, buddy->playerNumber, buddy->server.hostname, buddy->server.port);
	#endif
}

void removeBuddy(char *name) {
	// Find this buddy
	int count = 0;
	for (; count < IRC_MAX_BUDDIES; count++) {
		ircBuddy_t *buddy = &ircBuddies[count];

		// Buddy found
		if (buddy->infoValid && !strcmp(name, buddy->nick)) {
			buddy->infoValid = qfalse;
			buddy->invite = qfalse;

			addHudOutputMsg(hudinfo, "%s has left IRC fireteam", name);
			
			#ifdef ETH_DEBUG
				ethDebug("irc: remove buddy %s in slot %i", name, count);
			#endif
			return;
		}
	}
	
	// Can't find this buddy
	ethLog("irc: warning: can't remove buddy: buddy not found: %s", name);
}

ircmsgs_t ircMsgs[IRC_MAX_MSGS];
void ircCheckNetEvent() {
	static char incompleteBuffer[512];
	static int incompleteBufferSize = 0;

	char buffer[1024]; 
	memset(buffer, 0, sizeof(buffer));
	char *bufferPosition = buffer;

	int msgCount;

	// If we have an old incomplete buffer, add it to the current buffer
	if (incompleteBufferSize > 0) {
		strncpy(buffer, incompleteBuffer, incompleteBufferSize);
		bufferPosition = buffer + incompleteBufferSize;
	}

	int size = recv(ircSocket, bufferPosition, sizeof(buffer) - incompleteBufferSize - 1, 0);

	// No more need of incomplete buffer now
	incompleteBufferSize = 0;
	
	// Check if socket connected
	if (size == 0) {
		gameMessage(qfalse, "irc: disconnected from %s", ircServer.hostname);
		close(ircSocket);
		ircSocket = -1;
		return;
	}

	// If no data received
	if (size == -1) 
		return;

	#ifdef ETH_DEBUG
		ethDebug("irc: recv size: %i\n---buffer---\n%s\n---endBuffer---", size, buffer);
	#endif

	char *runningOffset = buffer;
	char *line;
	// Parse data received line by line
	while ((line = strsep(&runningOffset, "\n"))) {
		// If all lines receive complete
		if (!strlen(line))
			break;
		
		// Check for an incomplete line
		if (strcmp(line + strlen(line) - 1, "\r")) {
			#ifdef ETH_DEBUG
				ethDebug("incomplet line: [%s]", line);
			#endif

			// Backup incomplete data
			incompleteBufferSize = strlen(line);
			strncpy(incompleteBuffer, line, incompleteBufferSize);
			break;
		}

		// Remove trailing '\n'
		line[strlen(line) - 1] = '\0';

		#ifdef ETH_DEBUG
			ethDebug("irc: new line: [%s]", line);
		#endif
		
		// Get parts of the line
		char *partsOffset = line;
		char *part1 = strsep(&partsOffset, ":");
		char *part2 = strsep(&partsOffset, ":");
		char *part3 = partsOffset;

		#ifdef ETH_DEBUG
			ethDebug("irc: part1: [%s]", part1);
			ethDebug("irc: part2: [%s]", part2);
			ethDebug("irc: part3: [%s]", part3);
		#endif

		// Catch ping server request and send 'PONG'
		if (!strcmp(part1, IRC_PING)) {
			netSendText(ircSocket, "%s:%s", IRC_PONG, part2);
			#ifdef ETH_DEBUG
				ethDebug("irc: server request ping");
			#endif
			continue;
		}

		// Get parts from part2
		partsOffset	= part2;
		char *from = strsep(&partsOffset, " ");
		char *command = strsep(&partsOffset, " ");
		char *to = strsep(&partsOffset, " ");
	
		// Sanity check
		if (!from || !command)
			continue;

		// Discard host info on nickname
		if (strchr(from, '!'))
			*strchr(from, '!') = '\0';

		// Discard 'CTCP VERSION' message
		if (!strcmp(command, IRC_PRIVMSG) && !strcmp(part3, IRC_CTCP_VERSION))
			continue;

		// Register on the server
		if (!strcmp(command, IRC_REGISTRED)) {
			gameMessage(qfalse, "irc: connected to %s %s", ircServer.hostname, ircChannel);
			// Join channel
			netSendText(ircSocket, "%s %s", IRC_JOIN, ircChannel);
			netSendIrcStatus();
			netSendIrcChannel("!status");
			continue;
		}

		#ifdef ETH_DEBUG
			ethDebug("eth: irc: from: [%s]", from);
			ethDebug("eth: irc: command: [%s]", command);
			ethDebug("eth: irc: to: [%s]", to);
		#endif

		// New message
		if (!strcmp(command, IRC_PRIVMSG)) {
			// Bot status commands
			if (!strcmp(part3, IRC_BOT_STATUS)) {
				netSendIrcStatus();
				continue;
			// A bot give status
			} else if (!strncmp(part3, IRC_BOT_STATUS, strlen(IRC_BOT_STATUS))
					&& !strcmp(to, ircChannel)
					&& (getBuddySlot(from) != -1)) {
				updateBuddy(from, part3 + strlen(IRC_BOT_STATUS) + 1);
				continue;
			} else {
				msgCount = IRC_MAX_MSGS - 2;
				while (msgCount >= 0) {
					ircMsgs[msgCount + 1].type = ircMsgs[msgCount].type;
					strcpy(ircMsgs[msgCount + 1].nick, ircMsgs[msgCount].nick);
					strcpy(ircMsgs[msgCount + 1].msg, ircMsgs[msgCount].msg);
					msgCount--;		
				}
				// New message for me
				if (!strcmp(to, ircNick)) {
					// Check for medicbot message
					if (!strncmp(part3, MEDICBOT_REQUEST, sizeof(MEDICBOT_REQUEST))
							&& (getBuddySlot(from) != -1)) {
						doMedicBotCommand(ircBuddies[getBuddySlot(from)].playerNumber, part3 + sizeof(MEDICBOT_REQUEST));
					} else {
						// check for invite
						if (!strcmp(part3, IRC_BOT_INVITE) && (getBuddySlot(from) != -1)) {
							acceptInvite(getBuddySlot(from));
							continue;
						} else {
							// pm receive
							gameMessage(qfalse, "irc: pm: %s: %s", from, part3);
							ircMsgs[0].type = IRC_MSGTYPE_QUERY;
							if (!ircMsgsGeneral.show)
								addHudOutputMsg(hudwarning, "New IRC msg (query) from %s", from);
						}
					}
				// New message for channel
				} else if (!strcmp(to, ircChannel)) {
					ircMsgs[0].type = IRC_MSGTYPE_CHAN;
					gameMessage(qfalse, "irc: %s: %s", from, part3);

					if (!ircMsgsGeneral.show)
						addHudOutputMsg(hudinfo, "New IRC msg (%s) from %s", to, from);
				}

				strcpy(ircMsgs[0].nick, from);
				strcpy(ircMsgs[0].msg, part3);

				continue;
			}
		}
		
		// Server send list of names on channel
		if (!strcmp(command, IRC_LIST_NAMES)) {
			partsOffset	= part3;
			char *name;
			while ((name = strsep(&partsOffset, " "))) {
				#ifdef ETH_DEBUG
					ethDebug("eth: irc: name on channel: [%s]", name);
				#endif
				// Don't add no bot
				if (strncmp(name, IRC_BOT_PREFIX, strlen(IRC_BOT_PREFIX)))
					continue;
				
				addBuddy(name);
			}
			continue;
		}

		// Someone join the channel
		if (!strcmp(command, IRC_JOIN)
				&& strcmp(from, ircNick)	// It's not me
				&& !strncmp(from, IRC_BOT_PREFIX, strlen(IRC_BOT_PREFIX))) {	// And it's a bot
			addBuddy(from);
			#ifdef ETH_DEBUG
				ethDebug("eth: irc: buddy=%s join channel", from);
			#endif
		// Someone leave or quit the channel
		} else if (!strcmp(command, IRC_PART) || !strcmp(command, IRC_QUIT)) {
			removeBuddy(from);
			#ifdef ETH_DEBUG
				ethDebug("eth: irc: buddy=%s leave/quit channel", from);
			#endif
		// Someone change his name
		} else if (!strcmp(command, IRC_NICK)
				&& (getBuddySlot(from) != -1)) {	// And it's a existing bot
			strncpy(ircBuddies[getBuddySlot(from)].nick, part3, IRC_NICK_SIZE);
			#ifdef ETH_DEBUG
				ethDebug("eth: irc: buddy=%s change nick to %s", from, part3);
			#endif
		// Server sends channel topic or someone changes it
		} else if (!strcmp(command, IRC_TOPIC) || !strcmp(command, IRC_TOPIC_CHANGE)) {
			strncpy(seth.ircTopic, part3, IRC_TOPIC_SIZE);
		}
	}
	return;
}

void ircNextEvent() {
	// If user don't want to use irc
	if (seth.value[VAR_IRC] == 0) {
		if (ircSocket != -1)
			close(ircSocket);
		return;
	}

	// If user not playing no need to connect the bot
	if (!eth.hookLoad)
		return;
	
	// Slow down irc event
	#define IRC_EVENT_STEP 200	// in ms
	static struct timeval tv_lastIrcEvent;
	struct timeval tv_actual;
	gettimeofday(&tv_actual, NULL);
	struct timeval tv_next;
	gettimeofday(&tv_next, NULL);
	struct timeval tv_step;
	timerclear(&tv_step);
	
	tv_step.tv_usec = IRC_EVENT_STEP * 1000;
	timeradd(&tv_lastIrcEvent, &tv_step, &tv_next);

	// Don't too much irc event
	if (timercmp(&tv_next, &tv_actual, >))
		return;
	
	// Backup last irc event time
	memcpy(&tv_lastIrcEvent, &tv_actual, sizeof(tv_lastIrcEvent));
	
	// If not connected
	if (ircSocket == -1) {
		static time_t lastConnectTime = 0;
		
		// Wait a bit before first connection
		if (lastConnectTime == 0) {
			lastConnectTime = time(NULL);
			return;
		}
		
		// Block connection flood
		if ((lastConnectTime + IRC_RETRY_TIME) > time(NULL))
			return;

		// Send connect request
		lastConnectTime = time(NULL);
		ircConnect();
		memset(ircBuddies, 0, sizeof(ircBuddies));
		return;
	}
	
	// So check for next irc events
	ircCheckNetEvent();
}

void acceptInvite(int slot) {
	if (!seth.value[VAR_IRC_INVITE])
		return;

	ircBuddy_t *buddy = &ircBuddies[slot];

	buddy->invite = qtrue;
	seth.lastInvite = eth.cg_time;
	seth.lastInviteID = slot;
}

/*
==============================
CG_INIT - update our slot
==============================
*/

void ircCGinit() {
	if (strlen(ircNick)) {
		int slot = getBuddySlot(ircNick);
		if (slot == -1)
			return;
		ircBuddy_t *buddy = &ircBuddies[slot];
		if (!buddy->infoValid)
			return;

		qboolean changedServer = memcmp(&eth.server, &buddy->server, sizeof(server_t));
		// If we change server, inform other buddies about it
		// and update our buddy slot
		if (changedServer) {
			strcpy(buddy->server.hostname, eth.server.hostname);
			buddy->server.port = eth.server.port;
			buddy->playerNumber = eth.cg_clientNum;
			netSendIrcStatus();
		}
	}
}

/*
==============================
irc game commands
==============================
*/

void nickCommand() {
	if (syscall_UI_Argc() == 1) {
		gameMessage(qtrue, "Change nick of irc bot");
	}
	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		gameMessage(qtrue, "Usage: " ETH_CMD_PREFIX "irc_nick");
		return;
	}

	snprintf(ircNick, sizeof(ircNick), "%s%s", IRC_BOT_PREFIX, syscall_UI_Argv(1));

	if (ircSocket != -1)
		netSendText(ircSocket, "NICK %s", ircNick);
	else
		gameMessage(qtrue, "irc: not connected");
}

void sendCommand() {
	if (syscall_UI_Argc() == 1) {
		gameMessage(qtrue, "Send a message to the irc channel");
		gameMessage(qtrue, "Usage: " ETH_CMD_PREFIX "irc_send msg");
		return;
	}
	
	// Not connected
	if (ircSocket == -1)
		gameMessage(qtrue, "irc: not connected");

	// Get alls arguments
	char buffer[1024];
	
	strcpy(buffer, syscall_UI_Argv(1));

	int count;
	for (count = 2; count < syscall_UI_Argc(); count++) {
		strcat(buffer, " ");
		strcat(buffer, syscall_UI_Argv(count));
	}

	if (strlen(buffer))
		netSendIrcChannel(buffer);	// Remove first space
}

void messageSendCommand (void) {
	char	messageText[256];
	int	messageType;
	
	char *str;
	char *cmd;
	char send[512];

	// get values
	syscall_CG_Cvar_VariableStringBuffer( "cg_messageType", messageText, 256 );
	messageType = atoi( messageText );
	syscall_CG_Cvar_VariableStringBuffer( "cg_messageText", messageText, 256 );

	switch (messageType) {
		case 4:
			cmd = "eth_irc_send";
			break;
		case 3:
			cmd = "say_buddy";
			break;
		case 2:
			cmd = "say_team";
			break;
		default:
			cmd = "say";
			break;
	}

	// reset values
	str = "cg_messageText \"\"\n";
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, str);
	str = "cg_messageType \"\"\n";
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, str);

	// don't send empty messages
	if( messageText[ 0 ] == '\0' )
		return;

	sprintf(send, "%s \"%s\"\n", cmd, messageText);

	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, send);
}

void messageModePopup (void) {
	char *cmd = "messageMode\n";
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, cmd);
	cmd = "cg_messageType 4\n";
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, cmd);
}

void showBuddiesCommand (void) {
	orig_syscall(UI_PRINT, "^3ID ^1: ^3Player                                              Server\n");
	orig_syscall(UI_PRINT, "^1---------------------------------------------------------------\n");

	int count = 0;
	for (; count < IRC_MAX_BUDDIES; count++) {
		ircBuddy_t *buddy = &ircBuddies[count];

		if (!buddy->infoValid)
			continue;

		char player[256];
		char server[128];
		sprintf(server, "%s:%i", buddy->server.hostname, buddy->server.port);
		
		sprintf(player, "^%s%2d ^1:^7 %-30s       %21s\n", buddy->invite ? "2" : "7",count , buddy->nick, server);
		orig_syscall(UI_PRINT, player);
	}
}

void joinBuddyCommand (void) {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nJoin a server where is a buddy playing\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: \eth_joinbuddy #ID\n");
		return;
	}

	int id = atoi(syscall_UI_Argv(1));
	ircBuddy_t *buddy = &ircBuddies[id];

	if (!buddy->infoValid) {
		orig_syscall(UI_PRINT, "^nBuddy with that ID doesen't exist.\n");
		orig_syscall(UI_PRINT, "^nGet buddy IDs with \eth_showbuddies.\n");
		return;
	}

	qboolean isOnServer = !memcmp(&eth.server, &buddy->server, sizeof(server_t));

	if (isOnServer) {
		orig_syscall(UI_PRINT, "^nYou are already on the same server as buddy with that ID.\n");
		return;
	}
	char connect[128];
	sprintf(connect, "connect %s:%i\n", buddy->server.hostname, buddy->server.port);
	orig_syscall(UI_CMD_EXECUTETEXT, EXEC_APPEND, connect);
}

void inviteBuddyCommand (void) {
	if (syscall_UI_Argc() == 1)
		orig_syscall(UI_PRINT, "^nInvite a buddy to join you on the server\n");

	if ((syscall_UI_Argc() == 1) || (syscall_UI_Argc() > 2)) {
		orig_syscall(UI_PRINT, "^nUsage: \eth_invitebuddy #ID\n");
		return;
	}

	int id = getBuddySlot(ircNick);
	if (id == -1)
		return;
	ircBuddy_t *buddy = &ircBuddies[id];
	if (!buddy->infoValid) {
		orig_syscall(UI_PRINT, "^nYou are not connected to either server or irc\n");
		return;
	}

	id = atoi(syscall_UI_Argv(1));
	buddy = &ircBuddies[id];

	if (!buddy->infoValid) {
		orig_syscall(UI_PRINT, "^nBuddy with that ID doesen't exist.\n");
		orig_syscall(UI_PRINT, "^nGet buddy IDs with \\eth_showbuddies.\n");
		return;
	}

	qboolean isOnServer = !memcmp(&eth.server, &buddy->server, sizeof(server_t));
	if (isOnServer) {
		orig_syscall(UI_PRINT, "^nYou are already on the same server as buddy with that ID.\n");
		return;
	}
	char *buddyNick = buddy->nick;

	if (netSendIrcQuery(buddyNick, IRC_BOT_INVITE))
		orig_syscall(UI_PRINT, "^nInvited buddy.\n");
	else
		orig_syscall(UI_PRINT, "^nFailed inviting buddy.\n");
}

void registerIrcCommands() {
	// Replace existing messageSend cmd to work with IRC msgs
	orig_Cmd_RemoveCommand("messageSend");
	orig_Cmd_AddCommand("messageSend", &messageSendCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "irc_chat", &messageModePopup);

	orig_Cmd_AddCommand(ETH_CMD_PREFIX "irc_nick", &nickCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "irc_send", &sendCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "showbuddies", &showBuddiesCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "joinbuddy", &joinBuddyCommand);
	orig_Cmd_AddCommand(ETH_CMD_PREFIX "invitebuddy", &inviteBuddyCommand);
}
