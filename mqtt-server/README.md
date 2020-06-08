## MQTT Server (Dockerized)

### Vorbedingungen
- Eine Docker-Installation auf dem Zielserver

### Einrichtung
- Docker-Compose-Image klonen von https://github.com/thelebster/example-mosquitto-simple-auth-docker
- Im .env File sollte das Passwort des mosquitto users geändert werden
- Über ```docker-compose build``` und ```docker-compose up -d```wird der Server gebuildet und als Daemon ausgeführt

### 
