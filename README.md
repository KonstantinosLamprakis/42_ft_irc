## General concept: 
 - https://medium.com/@mohamedsarda2001/ft-irc-channels-and-command-management-ff1ff3758a0b
 - https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/
 - in details: https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

## IRC clients:
- we just use netcat(nc) as all the others need installation 
  - https://hexchat.github.io/
  - https://irssi.org/
  - https://halloy.chat/

## IRC servers:
 - on task's description
 - reading RFCv3 spesification
 - Libera Chat (see the example below)

## CPP command docs:
  - [poll](https://man7.org/linux/man-pages/man2/poll.2.html)
  
## code examples: 
- https://github.com/barimehdi77/ft_irc: 
  - old version which doesn't include commands like "INVITE"
  - contains File and FileTransfer which we dont need
  - contains DeezNuts which we dont need
 
## IRC client commands / playground:
- nc ip_here port_here
  - default port for irc is 6667
  - servers could be irc.devdungeon.com or irc.libera.chat
- USER username_here * * :real_name_here 
  - the 2 asterisks are the hostname and the server
  - you put colon to use spaces if needed on real name
  - username is used from server for authentication while nickname is the one the other users see
- NICK my_nick_name
  - could be changed many times while username can not
  - after USER and NICK command you are registered
- PRIVMSG user_name just the message
  - you don't need colon now even if it has spaces 
- JOIN channel_name
  - all channel names should start with hash
- PRIVMSG channel_name
  - when you send a message you dont see your messages again, only other people messages

## Architecture:
 - Classes:
   - Server:
     - independent if we agree on the format of the request in which the parser will take(just raw string from user input)
     - this does everything at the end, this is the core module, middleman who interacts with all clients
     - infinite loop with polling
     - on event either create new client either either forward the request to the parser
   - Request(for Parser):
     - indepentent if we sesify what request format expects from server and what args will pass on each command(a Request class with command and args and the cliendId(just the index) of spesific socket/client) 
     - taking user input, validate it, call the relevant command and return the response
   - User
   - Channel

## Guidelines:
 - you don't need persisted data so you can include the fd as well in this client class etc. 
 - maybe create some general functions(more likely as part of server) e.g. dispalyError, printMsg, getUserPreffix
 - i suggest to implement/add class attributes only when we actually need them and not in advance(as we may never use them at the end)
 - commands: one file per command with spesific name-convension e.g. kickCommand.cpp
 
## [Eval Sheet](https://github.com/mharriso/school21-checklists/blob/master/ng_5_ft_irc.pdf)

## IRC commands
[structure of the commands](https://www.rfc-editor.org/rfc/rfc1459.html)
- KICK
- INVITE
- TOPIC
- MODE
- NICK
- USER
- JOIN
- PRIVMSG
- NOTICE
- PASS
- QUIT