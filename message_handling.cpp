#include "message_handling.h"
void sending_message(int connection_socket_descriptor, char * tresc){
    /*
    *funkcja w petli wysyla kolejne fragmenty tresci wiadmosci,
    az nie wysle sumarycznie tyle bajtow, co ma tresc messagei.
    W kazdej iteracji petli while, w zmiennej temp zapisujemy
    tymczasowo coraz to mniejsza czesc tresci messagei
    */
    int write_result=0;
    int message_length=strlen(tresc);
    char * temp;
    do{
        temp = new char[message_length-write_result];
        strncpy(temp,tresc+write_result,message_length-write_result);
        write_result+=write(connection_socket_descriptor,temp,strlen(temp));
        delete temp;
    }while(write_result!=message_length);
    
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
            printf("Nastapil blad przy odczycie!\n");
            break;
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
