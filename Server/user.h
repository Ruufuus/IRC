#ifndef USER_H
#define USER_H
#include "const.h"
class user {
    private:
        std::string username;
        std::string color;
        int socket_description_id;
    public:
        void set_username(std::string username);
        void set_color(std::string color);
        void set_socket_description(int socket_description_id);
        int get_socket_description_id();
        std::string get_username();
        std::string get_color();
        user(int socket_description_id);
        user();
};


#endif