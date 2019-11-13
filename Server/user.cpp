#include "user.h"

        void user::set_username(std::string username){
            this->username=username;
        };
        void user::set_color(std::string color){
            this->color=color;
        };
        void user::set_socket_description(int socket_description_id){
            this->socket_description_id=socket_description_id;
        };
        std::string user::get_username(){
            return this->username;
        };
        std::string user::get_color(){
            return this->color;
        };
        int  user::get_socket_description_id(){
            return this->socket_description_id;
        };
        user::user(int socket_description_id){
            this->username="username";
            this->color="#ffffff";
            this->socket_description_id=socket_description_id;
        };
        user::user(){
            this->username="username";
            this->color="#ffffff";
            this->socket_description_id=-1;
        };

