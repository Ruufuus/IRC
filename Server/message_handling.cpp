#include "message_handling.h"
void sending_message(int connection_socket_descriptor, char * tresc){
    /*
    *funkcja w petli wysyla kolejne fragmenty tresci wiadmosci,
    az nie wysle sumarycznie tyle bajtow, co ma tresc messagei.
    W kazdej iteracji petli while, w zmiennej temp zapisujemy
    tymczasowo coraz to mniejsza czesc tresci messagei
    */
    int write_result=0;
    int current_write_result;
    do{
    std::string new_message = std::string(tresc);
    new_message=new_message.substr(write_result);
    current_write_result=write(connection_socket_descriptor,new_message.c_str(),strlen(new_message.c_str()));
     if(current_write_result<0)
        {
            fprintf(stderr, "Błąd przy próbie wyslania wiadomosci");
            exit(1);
        }
    write_result+=current_write_result;
    }while(write_result!=int(strlen(tresc)));
    
};


char * reading_message(int connection_socket_descriptor,bool * connected){
    /*
    Funkcja ta w kolejnych iteracjach petli while,
    odczytuje po jednym znaku, az nie natrafi na znak nowej linii
    Funkcja dodatkowo wykrywa czy uzytkownik sie nie rozlaczyl
    */
    char * buffor = new char[BUFF_SIZE];
    char * temp = new char;
    int read_result;
    memset(buffor,'\0',sizeof(char)*BUFF_SIZE);
    do{
        read_result=read(connection_socket_descriptor,temp,1);
        if(read_result<0)
        {
            fprintf(stderr, "Błąd przy próbie odczytu wiadomosci");
            exit(1);
        }
        else if(read_result==0){
            *connected=false;
            break;
        }
        strcat(buffor,temp);
    }while(strcmp(temp,"\n"));
    delete temp;
    return buffor;
};


int command_detection(char * message, char ** command){
    /*
    Funkcja ta porownuje pierwsze n znakow wiadomosci z 
    list komend predefiniowanych i zwraca command_number, 
    ktory indentyfikuje komende
    */
    int command_number = -1;
    
    for (int i=0;i<COMMAND_ARRAY_SIZE;i++)
    {
        if(!strncmp(message, command[i],strlen(command[i])))
            {
                sscanf(message,"%*s %s",message);
                command_number=i;
                break;
            }
    }

    return command_number;
};
