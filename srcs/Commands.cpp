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
    if (request.getArgs().size() < 1 || request.getArgs()[0] == "") { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Need more params.\n", user_id);
        return;
    }
    if (request.getArgs()[0] != this->_password) {
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
    if (request.getArgs().size() < 1 || request.getArgs()[0] == "") { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NONICKNAMEGIVEN, ":No nickname given.\n", user_id);
        return;
    }

    std::string nickname = request.getArgs()[0];
    std::string upperCaseNickname = toUppercase(nickname);
    for (int i = 1; i < this->_amnt_connections; i++) {
        if (i == user_id) continue; // ignore if user has already same nickname
        if (toUppercase(this->_users[i].get_nickname()) == upperCaseNickname) {
            this->print_error_to_user(Error::ERR_NICKNAMEINUSE, nickname + " :Nickname is already in use.\n", user_id);
            return;
        }
    }

    // check for invalid characters: starting with digit, contain space or :, # or &
    const std::string invalid_chars = ":#&!+" + SPACE;
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
    if (request.getArgs().size() < 4) { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Need more params.\n", user_id);
        return;
    }
    for (int i = 0; i < 4; i++) { // no param can be empty
        if (request.getArgs()[i] == "") {
            this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Need more params.\n", user_id);
            return;
        }
    }

    std::string username = request.getArgs()[0];
    const std::string realname = request.getArgs()[3]; // we ignore the 2nd and 3rd args, hostname and servername
    
    // check for invalid characters: starting with digit, contain space or :, # or &
    const std::string invalid_chars = ":#&!+" + SPACE;
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
    if (request.getArgs().size() < 1 ) { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, ":Not enough parameters.\n", user_id);
        return;
    }
    if (request.getArgs()[0] == "0") { // if user wants to leave all the channels channel
        // TODO(KL) remove user from all channels
        return;
    }

    // parsing channels format: channel1,channel2,channel3
    std::vector<std::string> channels;
    std::stringstream channels_stream(request.getArgs()[0]);
    std::string channel;
    while (std::getline(channels_stream, channel, ',')) {
        channels.push_back(channel);
    }

    // verify all channels name starts with # and not invalid chars + length
    const std::string invalid_chars = "\a," + SPACE;
    for (size_t i = 0; i < channels.size(); i++) {
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
        if (channels[i].size() == 0){
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
    }

    // parsing keys format: key1,key2
    std::vector<std::string> keys;
    if (request.getArgs().size() > 1) {
        std::stringstream keys_stream(request.getArgs()[1]);
        std::string key;
        while (std::getline(keys_stream, key, ',')) {
            channels.push_back(key);
        }
    }


    // if (this->_users[user_id].search_channel(channels[i]) != -1) { // user already joined the channel
    //     continue;
    // }

    // for (int i = 0; i < channels.size(); i++) {
    // }

    // TODO(KL) finish this command
    // check if user reached CHANNEL_PER_USER_LIMIT
}