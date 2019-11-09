#include "user.h"
class user{
        void set_username(char * username){
            this.username=username;
        };
        void set(char * color){
            this.color=color;
        };
        char * get_username(){
            return this.username;
        }
        char * get_color(){
            return this.color;
        }
        char * get_socket_description_id(){
            return this.socket_description_id;
        }
        user(int socket_description_id){
            this.socket_description_id=socket_description_id;
        }
};
