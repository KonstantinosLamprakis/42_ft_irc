#include "../headers/Server.hpp"
#include "../headers/User.hpp"
#include "../headers/Request.hpp"

void Server::pass(Request request, int fd) {
    if (this->_users[fd].is_authenticated()) {
        this->print_msg_to_user("Error: User is already authenticated.\n", fd);
        return;
    }
    if (request.getArgs().size() != 1) {
        this->print_msg_to_user("Error: PASS command requires exactly one argument.\n", fd);
        return;
    }
    if (request.getArgs()[0] != this->_password) {
        this->print_msg_to_user("Error: Incorrect password.\n", fd);
        return;
    }
    this->_users[fd].set_authenticated(true);
}