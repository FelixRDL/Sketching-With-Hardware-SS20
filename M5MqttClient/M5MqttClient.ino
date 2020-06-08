#include <M5Stack.h>
#include <WiFi.h>
#include <PubSubClient.h>
const char* SSID = "REDACTED";
const char* PW = "REDACTED";
const char* MQTT_BROKER = "REDACTED";
const char* MQTT_CLIENT_NAME = "REDACTED";
const char* MQTT_CLIENT_PASSWORD= "REDACTED";
WiFiClient wifi;
PubSubClient mqtt(wifi);

// the setup routine runs once when M5Stack starts up
void setup(){
  Serial.begin(115200);

  // Initialize the M5Stack object
  M5.begin();

  setup_wifi();
  mqtt.setServer(MQTT_BROKER, 1883);
  mqtt.setCallback(mqtt_callback);
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
    
  // LCD display
  M5.Lcd.print("- - INITIALIZATION COMPLETE - -");
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

void setup_mqtt() {
    while (!mqtt.connected()) {
        Serial.println("Reconnecting MQTT...");
        if (!mqtt.connect("ESP8266Client", MQTT_CLIENT_NAME, MQTT_CLIENT_PASSWORD)) {
            Serial.print("failed, rc=");
            Serial.print(mqtt.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    mqtt.subscribe("/chat");
    Serial.println("MQTT Connected...");
    mqtt.publish("/chat", "Hallo in die Runde! :)");
}

// the loop routine runs over and over again forever
void loop() {
    if (!mqtt.connected()) {
        setup_mqtt();
    }
    mqtt.loop();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        msg[i] = (char)payload[i];
    }
    M5.Lcd.println();

    msg[length] = '\0';

    M5.Lcd.print(">");   
    M5.Lcd.println(topic);
    M5.Lcd.print(">  ");   
    M5.Lcd.println(msg);
}
