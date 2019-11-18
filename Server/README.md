# **serwer TCP**
## Wymagania do kompilacji i działania serwera.
  - System wspierający API BSD sockets,
  - Zainstalowany kompilator g++
  
## W celu kompilacji serwera należy wykonać polecenie w terminalu:
  ```
  chmod u+x <Project_path>/Server/skrypt.sh 
  <Project_path>/skrypth.sh
  ```

## W celu odpalenie serwera należy wykonać polecenie w terminalu:
  ```
  <Project_path>/main.exe <port_number>
  ```
  - <Project_path> - ścieżka do folderu z projektem, ewentualnie można zastąpić i użyć ./<command>,
  gdy znajdujemy się w folderze z plikiem, na którym chcemy wykonywać operacje
  - <port_number> - port, na którym serwer będzie odpalony


## komendy usera
  - $join <channel_name> -dołącza do kanału lub tworzy kanał, gdy kanał o zadanej nazwie nie istnieje
  - $room_list -wyświetla listę pokoi
  - $user_list -wyświetla listę userów podłączonych do pokoju
  - $username <username> -ustawia nazwe użytkownika
  - $color  -ustawia kolor wiadomości użytkownika (format to HEX lub niektóre angielskie nazwy kolorów)
  - $leave  -odłącza użytkownika od serwera

## Informacje o błędach wysłane do usera
  - $error <error_message> - wysyła do użytkownika komunikat błędu

## komendy serwera
  - stop -wyłącza serwer

## format zwykłej wiadomości wysyłanej przez serwer
  - HH:MM:SS username@color <message content>
