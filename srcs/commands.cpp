#include "../headers/Server.hpp"
#include "../headers/User.hpp"
#include "../headers/Request.hpp"

void Server::pass(Request request, int user_id) {
    if (this->_users[user_id].is_authenticated()) {
        this->print_error_to_user(Error::ERR_ALREADYREGISTERED, "User is already registered.\n", user_id);
        return;
    }
    if (request.getArgs().size() < 1) { // if more than 1, server only use the 1rst one
        this->print_error_to_user(Error::ERR_NEEDMOREPARAMS, "Need more params.\n", user_id);
        return;
    }
    if (request.getArgs()[0] != this->_password) {
        this->print_error_to_user(Error::ERR_PASSWDMISMATCH, "Password Incorrect.\n", user_id);
        return;
    }
    this->print_msg_to_user("User Authenticated.\n", user_id);
    this->_users[user_id].set_authenticated(true);
}

void Server::nick(Request request, int user_id) {
    this->print_msg_to_user(request.getCommand(), user_id);
}

void Server::user(Request request, int user_id) {
    this->print_msg_to_user(request.getCommand(), user_id);
}

void Server::quit(Request request, int user_id) {
    (void)request;
	close_connection(user_id);
}