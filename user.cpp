#include "user.h"
class user{
    private:
        char * username;
        char * color;
        int socket_description_id;
    public:
        void set_username(char * username);
        void set(char * color);
        char * get_username();
        char * get_color();
        user(int socket_description_id);
};
