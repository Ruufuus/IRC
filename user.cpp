#include "user.h"

        void user::set_username(char * username){
            this->username=username;
        };
        void user::set_color(char * color){
            this->color=color;
        };
        void user::set_socket_description(int socket_description_id){
            this->socket_description_id=socket_description_id;
        }
        char * user::get_username(){
            return this->username;
        }
        char * user::get_color(){
            return this->color;
        }
        int  user::get_socket_description_id(){
            return this->socket_description_id;
        }
        user::user(int socket_description_id){
            strcpy(this->username,"username");
            strcpy(this->color,"#ffffff");
            this->socket_description_id=socket_description_id;
        }
        user::user(){
            strcpy(this->username,"username");
            strcpy(this->color,"#ffffff");
            this->socket_description_id=-1;
        };

