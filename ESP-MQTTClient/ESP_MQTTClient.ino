/**
 * Used this tutorial
 * https://smarthome-blogger.de/tutorial/esp8266-mqtt-tutorial/
 */
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
ESP8266WiFiMulti WiFiMulti;
const char* SSID = "REDACTED";
const char* PW = "REDACTED";
const char* MQTT_BROKER = "REDACTED";

const char* CLIENT_NAME = "REDACTED";
const char* CLIENT_PASSWORD= "REDACTED";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];


#include "DHT.h" //DHT Bibliothek
//Pin an welchem der DHT11 Sensor angeschlossen ist.
//Beim DHT11 Shield ist es der digitale Pin D4.
#define DHTPIN D4
//Festlegen welcher Typ von DHT Sensor verwendet wird.
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

long lastMeasureTime = millis();
int interval = 10000;

void setup() {
  Serial.begin(115200);
  // WiFiMulti.addAP(SSID, PW);
  setup_wifi();
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(D4, OUTPUT);
  // pinMode(PIR, INPUT_PULLUP);

  dht.begin(); //DHT Kommunikation beginnen.

}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PW);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();

    msg[length] = '\0';
    Serial.println(msg);

    if(strcmp(msg,"on")==0){
        digitalWrite(BUILTIN_LED, LOW);
    }
    else if(strcmp(msg,"off")==0){
        digitalWrite(BUILTIN_LED, HIGH);
    }
    else if(strcmp(msg,"blink")==0){
      blink();
    }
}

void blink(){
  for(int i = 0; i < 3; i++) {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
  }
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Reconnecting MQTT...");
        if (!client.connect("ESP8266Client", CLIENT_NAME, CLIENT_PASSWORD)) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    client.subscribe("/lamperl");
    Serial.println("MQTT Connected...");
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    if(millis() - lastMeasureTime > interval) {
        // https://draeger-it.blog/wemos-d1-mini-shield-dht11-sensor/
        //lesen der Luftfeuchtigkeit
        double luftfeuchtigkeit = dht.readHumidity();
        //lesen der Temperatur in Grad Celsius
        double temperaturC = dht.readTemperature();
        client.publish("/senser/temperature", String(temperaturC, 2).c_str());
        client.publish("/senser/humidity", String(luftfeuchtigkeit, 2).c_str());
        lastMeasureTime = millis();
    }
}
