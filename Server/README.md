# **Serwer TCP obsługujący system chatu typu IRC**
## Uwagi
  - Serwer nie kontroluje długości wpisywanych username'ów i wiadomości,
  - Dozwolone są wyłacznie jednowyrazowe username'y oraz nazwy pokoi,
  - W pliku const.h są umieszczone stałe, które administrator serwera może zmieniać, 
  aby dopasować serwer do swoich potrzeb. (Należy po zmianach przekompilować projekt)
  - Serwer po stworzeniu nowego kanału, wyśle do wszystkich podłączonych użytkowników listę kanałów,
  - Serwer po zmianie przez użytkownika username'u lub koloru wyślę do wszystkich użytkowników, podłączonych do tego samego kanału, listę użytkowników.
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
  - <Project_path> - ścieżka do folderu z projektem, ewentualnie można zastąpić i użyć ./\<command\>,
  gdy znajdujemy się w folderze z plikiem, na którym chcemy wykonywać operacje
  - <port_number> - port, na którym serwer będzie odpalony


## Komendy usera
  - $join \<channel_name\> -dołącza do kanału lub tworzy kanał, gdy kanał o zadanej nazwie nie istnieje,
  - $room_list -wyświetla listę pokoi w formacie "$room_list <\room_list>\", gdzie każdy pokój jest wymieniony z nazwy po spacji,
  - $user_list -wyświetla listę userów podłączonych do pokoju w formacie "$user_list <\user_list>\", gdzie każdy użytkownik jest wymieniony z <\username>\@<\color>\ po spacji.
  - $username \<username\> -ustawia nazwe użytkownika,
  - $color  \<color\>-ustawia kolor wiadomości użytkownika (format to HEX lub niektóre angielskie nazwy kolorów),
  - $leave  -odłącza użytkownika od serwera,
  - <\message_content\> -każda wiadomość usera, która nie jest poprzedzona komendą, zostanie rozesłana po userach podłączonych do pokoju.

## Informacje o błędach wysłane do usera
  - $error \<error_message\> - wysyła do użytkownika komunikat błędu

## Komendy serwera
  - stop -wyłącza serwer

## Format zwykłej wiadomości wysyłanej przez serwer
  - username@color HH:MM:SS \<message content\>
