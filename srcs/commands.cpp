#include "../headers/Server.hpp"
#include "../headers/User.hpp"
#include "../headers/Request.hpp"

void Server::pass(Request request, int user_id) {
    // TODO add noumeric on reply, check the order of the validation, optimize messages
    if (this->_users[user_id].is_authenticated()) {
        this->print_msg_to_user("Error: User is already authenticated.\n", user_id);
        return;
    }
    if (request.getArgs().size() != 1) {
        this->print_msg_to_user("Error: PASS command requires exactly one argument.\n", user_id);
        return;
    }
    if (request.getArgs()[0] != this->_password) {
        this->print_msg_to_user("Error: Incorrect password.\n", user_id);
        return;
    }
    this->print_msg_to_user("User Authenticated.\n", user_id);
    this->_users[user_id].set_authenticated(true);
}