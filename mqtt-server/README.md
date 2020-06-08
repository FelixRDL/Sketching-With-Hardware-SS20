## MQTT Server (Dockerized)

### Vorbedingungen
- Eine Docker-Installation auf dem Zielserver
- Eine Installation von Mosquitto auf dem Zielserver

### Einrichtung
- Docker-Compose-Image klonen von https://github.com/thelebster/example-mosquitto-simple-auth-docker
- Im .env File sollte das Passwort des mosquitto users geändert werden
- In ./conf/mosquitto.conf muss der Wert für ```password_file``` auf ```/mosquitto/config/mosquitto.passwd``` gesetzt werden
- Über ```docker-compose build``` und ```docker-compose up -d```wird der Server gebuildet und als Daemon ausgeführt

### User anlegen
- Im Ordner ./conf muss folgender Befehl ausgeführt werden ```mosquitto_passwd -b ./mosquitto.passwd <username> <password>```
- Anschließend muss der Server eventuell neu gebuildet und gestartet werden
