#include "const.h"
//funkcja wykrywa czy message zaczyna sie od ciagu znakow zawartych w komendzie
//char * message - tresc wiadomosci
//char ** command - lista kommend
//zwraca numer komendy w liscie lub -1, gdy jej nie znaleziono
int command_detection(char * message, char ** command);

//funkcja odbierajaca messagei z ochrona przeciw fragmentacji
//int c_s_d deskryptor socketu z ktorego bedzie odczytywana wiadomosc
//bool * connected - referencja na zmienna przechowujaca informacje, czy uzytkownik jest polaczony
//zwraca odczytana wiadomosc
char * reading_message(int connection_socket_descriptor,bool * connected);

//funkcja wysylajaca message z ochrona przeciw fragmentacji
//int c_s_d deskryptor soketu na ktory bedzie wysylana wiadomosc
//char * tres - zmienna przechowujaca tresc wiadomosci do wyslania
void sending_message(int connection_socket_descriptor ,char * buffor);

