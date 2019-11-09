#ifndef USER_H
#define USER_H
#include "const.h"
class user {
    private:
        char * username;
        char * color;
        int socket_description_id;
    public:
        void set_username(char * username);
        void set_color(char * color);
        void set_socket_description(int socket_description_id);
        int get_socket_description_id();
        char * get_username();
        char * get_color();
        user(int socket_description_id);
        user();
};


#endif