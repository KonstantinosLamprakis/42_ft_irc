#include "../headers/Commands.hpp"

/**
 * @brief PASS command allows the client to authenticate with the server
 * 
 * On success it doesn't print any feedback to the client
 * 
 * Edge cases:
 * - PASS : -> error
 * - PASS -> error
 * - PASS wrong_password -> error
 * - PASS correct_password -> returns no message and register the user
 * - run PASS after authentication -> error
 * - PASS password arg1 arg2 -> should ignore args and execute normally
 * 
 * @param request 
 * @param user_id 
 */
void Server::pass(Request request, int user_id) {
    if (this->_users[user_id].is_authenticated()) {
        this->print_error_to_user(Error::ERR_ALREADYREGISTERED, ":User is already registered.\n", user_id);
        return;
    }
    if (request.get_args().size() < 1 || request.get_args()[0] == "") { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Need more params.\n", user_id);
        return;
    }
    if (request.get_args()[0] != this->_password) {
        this->print_error_to_user(Error::ERR_PASSWDMISMATCH, ":Password Incorrect.\n", user_id);
        return;
    }
    this->_users[user_id].set_authenticated(true);
}

/**
 * @brief NICK command alows the client to change his nickname
 * 
 * Edge cases:
 * 
 * - NICK : -> error
 * - NICK -> error
 * - NICK invalid_characters -> error
 * - NICK same_nickname_as_other_user -> error
 * - NICK nickname_more_than_16_chars -> works but its truncated to the first 16 chars
 * - run NICK before authothentication(PASS) -> error
 * - run NICK before registration(PASS + NICK + USER) -> prints nothing, updates nickname
 * - run NICK after registration -> prints and updates nickname
 * - run NICK with extremely large nickname -> TODO(KL)
 * - run NICK with same nickname -> does nothing
 * - run NICK with same nickname but different capitalization -> updates nickname
 * 
 * @param request 
 * @param user_id 
 */
void Server::nick(Request request, int user_id) {
    if (!this->_users[user_id].is_authenticated()) {
        this->print_error_to_user(Error::ERR_NOAUTHENTICATION, ":You need to authenticate first.\n", user_id);
        return;
    }
    if (request.get_args().size() < 1 || request.get_args()[0] == "") { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NONICKNAMEGIVEN, ":No nickname given.\n", user_id);
        return;
    }

    std::string nickname = request.get_args()[0];
    std::string uppercase_nickname = to_uppercase(nickname);
    for (int i = 1; i < this->_amnt_connections; i++) {
        if (i == user_id) continue; // ignore if user has already same nickname
        if (to_uppercase(this->_users[i].get_nickname()) == uppercase_nickname) {
            this->print_error_to_user(Error::ERR_NICKNAMEINUSE, nickname + " :Nickname is already in use.\n", user_id);
            return;
        }
    }

    // check for invalid characters: starting with digit, contain space or :, # or &
    const std::string invalid_chars = ":" + CHANNEL_MODE + SPACE;
    if (nickname.find_first_of(invalid_chars) != std::string::npos || std::isdigit(static_cast<unsigned char>(nickname[0]))){
        this->print_error_to_user(Error::ERR_ERRONEUSNICKNAME, nickname + " :Erroneous Nickname.\n", user_id);
        return;
	}

    if (nickname.size() > 16) nickname.resize(16); // nickname is truncated in case its bigger than 16 chars

    // this is case-sensitive check, user can change to same nickname but different capitalization
    if (nickname == this->_users[user_id].get_nickname()) return; 
    
    if (this->_users[user_id].is_registered()) { // if user is already registered, we update his nickname and print a message
        const std::string old_nickname = this->_users[user_id].get_nickname();
        this->_users[user_id].set_nickname(nickname);
        this->print_msg_to_user(":" + old_nickname + "! " + "NICK :" + nickname + "\n", user_id);
    } else { // if user is not still registered, we just update his nickname and then check again if now he is registered
        this->_users[user_id].set_nickname(nickname);
        if (this->_users[user_id].is_registered()) {
            // print welcome message RPL_WELCOME (001)
            std::string welcome_msg = ":" + SERVER_NAME + " 001 " + nickname + " :Welcome to " + SERVER_NAME +" Internet Relay Chat Network " + nickname + "\n";
            this->print_msg_to_user(welcome_msg, user_id);
        }
    }
}

/**
 * @brief USER commands used to register the user(in combination with NICK command). 
 * 1rst args is the username while 4th arg is the realname
 * 2nd and 3rd args are ignored(hostname - servername)
 * 
 * Edge cases:
 * - run USER with less args or empty args e.g. : -> error
 * - run USER before authentication(PASS) -> error
 * - run USER multiple times before registration -> its fine, overwrite the old values
 * - run USER after registration -> error
 * - USER username_more_than_16_chars -> works but its truncated to the first 16 chars
 * - USER invalid_char_username host server realname -> error and close connection
 * - invalid chars on hostname, servername, realname -> execute normally, no error, there are no invalid chars for them
 * 
 * @param request 
 * @param user_id 
 */
void Server::user(Request request, int user_id) {
    if (this->_users[user_id].is_registered()) {
        this->print_error_to_user(Error::ERR_ALREADYREGISTERED, ":You are already registered.\n", user_id);
        return;
    }
    if (!this->_users[user_id].is_authenticated()) {
        this->print_error_to_user(Error::ERR_NOAUTHENTICATION, ":You need to authenticate first.\n", user_id);
        return;
    }
    if (request.get_args().size() < 4) { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Need more params.\n", user_id);
        return;
    }
    for (int i = 0; i < 4; i++) { // no param can be empty
        if (request.get_args()[i] == "") {
            this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Need more params.\n", user_id);
            return;
        }
    }

    std::string username = request.get_args()[0];
    const std::string realname = request.get_args()[3]; // we ignore the 2nd and 3rd args, hostname and servername
    
    // check for invalid characters: starting with digit, contain space or :, # or &
    const std::string invalid_chars = ":" + CHANNEL_MODE + SPACE;
    if (username.find_first_of(invalid_chars) != std::string::npos || std::isdigit(static_cast<unsigned char>(username[0]))){
        this->print_msg_to_user("Invalid username[~" + username + "].\n", user_id);
        this->close_connection(user_id);
        return;
    }

    if (username.size() > 16) username.resize(16);
    this->_users[user_id].set_username(username);
    this->_users[user_id].set_fullname(realname);

    if (this->_users[user_id].is_registered()) {
        // print welcome message RPL_WELCOME (001)
        std::string welcome_msg = ":" + SERVER_NAME + " 001 " + this->_users[user_id].get_nickname() + " :Welcome to " + SERVER_NAME +" Internet Relay Chat Network " + this->_users[user_id].get_nickname() + "\n";
        this->print_msg_to_user(welcome_msg, user_id);
    }
}

/**
 * @brief server should close the connection with the client, nothing more
 * 
 * @param request no needed but have it for future use
 * @param user_id 
 */
void Server::quit(Request request, int user_id) {
    (void)request;
	close_connection(user_id);
}

/**
 * @brief used from a client to join a new/existed channel
 * 
 * Edge cases:
 * - user wants to join multiple channels at the same time -> error as our SERVER limit is 1
 * - user want to join to existed channel 
 * - user want to join to non-existed channel -> channel created, he bocomes moderator
 * - user joins a channel using key(password)
 * 
 * - JOIN -> error not enough params
 * - JOIN channel_not_start_# -> error no such channel
 * - JOIN #channel1,,,, -> no error, just join #channel1
 * - JOIN #channel_more_than_50_chars -> error illegal channel name
 * 
 * - Channels can have many modes (#&!+), we only support # mode
 *  - we will not support &, ! and + as we don't communicate with other servers, they are optional on IRC and on task description
 * 
 * Format: JOIN channels keys or JOIN 0
 * @param request 
 * @param user_id 
 */
void Server::join(Request request, int user_id) {
    if (!this->_users[user_id].is_registered()) {
        this->print_error_to_user(Error::ERR_NOTREGISTERED, ":You have not registered.\n", user_id);
        return;
    }
    if (request.get_args().size() < 1 ) { // we expect 1 - 2 args, if there are more we ignore them
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Not enough parameters.\n", user_id);
        return;
    }
    if (request.get_args()[0] == "0") { // if user wants to leave all the channels
        for (unsigned long i = 0; i < this->_channels.size(); i++) {
            if (this->_channels[i].remove_user(this->_users[user_id].get_nickname())){
                this->print_msg_to_user(":" + this->_users[user_id].get_nickname() + "!" + this->_users[user_id].get_username() + " PART " + this->_channels[i].get_name() + "\n", user_id);
            }
            this->_users[user_id].remove_channel(this->_channels[i].get_name());
        }
        return;
    }

    // parsing channels format: channel1,channel2,channel3
    std::vector<std::string> channels;
    std::stringstream channels_stream(request.get_args()[0]);
    std::string channel;
    while (std::getline(channels_stream, channel, ',')) {
        channels.push_back(channel);
    }

    // verify all channels name starts with # and not invalid chars + length
    const std::string invalid_chars = "\a," + SPACE;
    for (size_t i = 0; i < channels.size(); i++) {
        if (channels[i].size() == 0){
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
        if (channels[i].size() > 50){
            this->print_error_to_user(Error::ERR_ILLEGALCHANNELNAME, channels[i] + " :Illegal channel name.\n", user_id);
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
        if (channels[i].find_first_of(invalid_chars) != std::string::npos){
            this->print_error_to_user(Error::ERR_ILLEGALCHANNELNAME, channels[i] + " :Illegal channel name.\n", user_id);
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
        if (channels[i][0] != '#'){
            this->print_error_to_user(Error::ERR_NOSUCHCHANNEL, channels[i] + " :No such channel.\n", user_id);
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
    }

    // parsing keys format: key1,key2
    std::vector<std::string> keys;
    if (request.get_args().size() > 1) {
        std::stringstream keys_stream(request.get_args()[1]);
        std::string key;
        while (std::getline(keys_stream, key, ',')) {
            channels.push_back(key);
        }
    }

    for (unsigned long i = 0; i < channels.size(); i++) {
        if (this->_users[user_id].is_user_in_channel(channels[i])) { // user already joined the channel
            continue;
        }

        if (this->_users[user_id].get_channel_number() == CHANNEL_PER_USER_LIMIT) {
            this->print_error_to_user(Error::ERR_TOOMANYCHANNELS, channels[i] + " :You have joined too many channels.\n", user_id);
            continue;
        }

        int channel_index = this->get_channel_index(channels[i]);
        if (channel_index == -1){
            std::string key = "";
            if (keys.size() > i + 1) {
                key = keys[i];
            }
            this->_channels.push_back(Channel(channels[i], key, this->_users[user_id].get_nickname()));
            this->_users[user_id].add_channel(channels[i]);
            this->print_msg_to_user(":" + this->_users[user_id].get_nickname() + "!" + this->_users[user_id].get_username() + " JOIN :" + channels[i] + "\n", user_id);
            return;
        }  

        try {
            std::string key = "";
            if (keys.size() > i + 1) {
                key = keys[i];
            }
            this->_channels[channel_index].add_user(this->_users[user_id].get_nickname(), key);
            this->_users[user_id].add_channel(channels[i]);
        } catch (const MaxNumberOfUsersInChannel &e) {
            this->print_error_to_user(Error::ERR_CHANNELISFULL, channels[i] + ":Cannot join channel (+l). Channel is full.\n", user_id);
        } catch (const IncorrectKeyForChannel &e) {
            this->print_error_to_user(Error::ERR_BADCHANNELKEY, channels[i] + " :Cannot join channel (+k). Incorrect key.\n", user_id);
        }
        // TODO(KL) handle the case of invite only channels
        // TODO(KL) print message when new user joins the channel
    }
}

/**
 * @brief sends a message to another user or to a channel
 * 
 * Edge cases:
 * - channel doesn't exist -> error
 * - user doesn't exist -> error
 * - user is not in the channel -> error
 * - user sends a message to himself -> works
 * - user sends a message to multiple users -> works
 * - user sends a message to multiple channels -> works
 * - user sends message to more than MAX_TARGETS_ON_PRIVMSG -> error
 * - user sends a message to a user not registered -> error
 * - user has duplicates values on the targets -> send only once
 * - when a user sends a message to a channel:
 *   - user should be already in this channel, otherwise ->
 *   - sender shouldn't get the message, all other user should get it
 *   - user can use ~ to send message only to founder of the channel
 *   - suer can use @ to send message only to operators of the channel
 * 
 * @param request 
 * @param user_id 
 */
void Server::privmsg(Request request, int user_id){
    if (!this->_users[user_id].is_registered()) {
        this->print_error_to_user(Error::ERR_NOTREGISTERED, ":You have not registered.\n", user_id);
        return;
    }
    if (request.get_args().size() < 1 ) {
        this->print_error_to_user(Error::ERR_NORECIPIENT, ":No recipient given. (PRIVMSG)\n", user_id);
        return;
    } else if (request.get_args().size() < 2 ) {
        this->print_error_to_user(Error::ERR_NOTEXTTOSEND, ":No text to send.\n", user_id);
        return;
    }

    // parsing targets in format: target1,target2
    std::vector<std::string> targets;
    std::stringstream targets_stream(request.get_args()[0]);
    std::string target;
    while (std::getline(targets_stream, target, ',')) {
        targets.push_back(target);
    }

    remove_duplicates(targets);
    for (size_t i = 0; i < targets.size(); i++) {
        if (targets[i].size() == 0){
            continue;
        }
        if (i + 1 > MAX_TARGETS_ON_PRIVMSG){
            this->print_error_to_user(Error::ERR_TOOMANYTARGETS, ":Too many targets.\n", user_id);
            break;
        }
        try{
            std::string msg = ":" + this->_users[user_id].get_nickname() + "!" + " PRIVMSG " + targets[i] + " :" + request.get_args()[1] + "\n";
            if (CHANNEL_PREFIX.find(targets[i][0]) != std::string::npos){
                // we do not support prefixes as its not mandatory and also not supported from irc.libera.chat
                this->print_error_to_user(Error::ERR_NOSUCHNICK, targets[i] + " :No such nick/channel.\n", user_id);
            } else if (CHANNEL_MODE.find(targets[i][0]) != std::string::npos){
                this->print_msg_to_channel(msg, targets[i], this->_users[user_id].get_nickname());
            } else { 
                this->print_msg_to_user_with_nickname(msg, targets[i]);
            }
        }catch (const ChannelNotFound &e) {
            this->print_error_to_user(Error::ERR_NOSUCHNICK, targets[i] + " :No such nick/channel.\n", user_id);
        } catch (const UserNotFound &e) {
            this->print_error_to_user(Error::ERR_NOSUCHNICK, targets[i] + " :No such nick/channel.\n", user_id);
        } catch (const UserNotInChannel &e) {
            this->print_error_to_user(Error::ERR_CANNOTSENDTOCHAN, targets[i] + ":Cannot send to nick/channel.\n", user_id);
        }
    }
}

void Server::mode(Request request, int user_id){
    if (!this->_users[user_id].is_registered()) {
        this->print_error_to_user(Error::ERR_NOTREGISTERED, ":You have not registered.\n", user_id);
        return;
    }
    if (request.get_args().size() < 1 || request.get_args()[0] == "" ) { // we expect 1 - 2 args, if there are more we ignore them
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Not enough parameters.\n", user_id);
        return;
    }
    std::string target = request.get_args()[0];
    if (to_uppercase(target) == to_uppercase(this->_users[user_id].get_nickname())){
        // we only support MODE command for channel, not users as its mentioned in task's description
        this->print_error_to_user(Error::ERR_MODENOTFORCHANNEL, ":MODE comand is only supported for channels, not users.\n", user_id);
        return;
    } else if (this->does_user_exist(target)){
        this->print_error_to_user(Error::ERR_USERSDONTMATCH, ":Can't change mode for other users.\n", user_id);
        return;
    }
    // handle user mode for channel
    int channel_index = this->get_channel_index(target);
    if (channel_index == -1){
        this->print_error_to_user(Error::ERR_NOSUCHCHANNEL, target + " :No such channel.\n", user_id);
        return;
    }
    if (request.get_args().size() == 1 || request.get_args()[1] == ""){
        this->print_reply_to_user(RPL::RPL_CHANNELMODEIS, target + " +" + this->_channels[channel_index].get_modes() + "\n", user_id);
        this->print_reply_to_user(RPL::RPL_CREATIONTIME, target + " " + this->_channels[channel_index].get_creation_timestamp() + "\n", user_id);
    }
    // TODO(KL) implement the rest argiuments
}