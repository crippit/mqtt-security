#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// this constant won't change:
const int buttonPin = D2;    // the pin that the pushbutton is attached to
const int door2Pin = D3;
const int door3Pin = D6;
const int ledPin = D4;       // the pin that the LED is attached to

// Current State:
int buttonState = 0; // current state of the button
int door2State = 0;
int door3State = 0;
//Last State
int lastButtonState = 0;     // previous state of the button
int lastDoor2State = 0;
int lastDoor3State = 0;

//WiFi & MQTT 
const char* ssid = "SSID;
const char* password = "WIFI PASSWORD";
const char* mqtt_server = "SERVER_IP";
const char* mqtt_server_user = "MQTT_USERNAME";
const char* mqtt_server_pass = "MQTT_PASSWORD";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(door2Pin, INPUT_PULLUP);
  pinMode(door3Pin, INPUT_PULLUP);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.connect("D1 Mini", mqtt_server_user, mqtt_server_pass);
  if (client.connect("D1 Mini", mqtt_server_user, mqtt_server_pass)){
    Serial.println("connected to MQTT");
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("D1 Mini", mqtt_server_user, mqtt_server_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  //Check if MQTT is connected 
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //Check if WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Need to Reconnect WiFi");
    setup_wifi();
  }
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  door2State = digitalRead(door2Pin);
  door3State = digitalRead(door3Pin);
  //Door 1 = Front Door
  //Door 2 = Back Door
  //Door 3 = Garage Door
  // compare the door to its previous state
    if (buttonState != lastButtonState) {
    // if the door state has changed
    if (buttonState == LOW) {
            Serial.println("Door Open");
            client.publish("ha/door/door1","Open");
            Serial.println("Published Door 1 Open");
         } else {
      Serial.println("Door Closed");
      client.publish("ha/door/door1","Closed");
      Serial.println("Published Door 1 Closed");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
   lastButtonState = buttonState;

  //##### Door2 #######
  if (door2State != lastDoor2State) {
    // if the door state has changed
    if (door2State == LOW) {
      Serial.println("Door 2 Open");
      client.publish("ha/door/door2","Open");
          } else {
      Serial.println("Door 2 Closed");
      client.publish("ha/door/door2","Closed");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastDoor2State = door2State;

  //##### Door3 ######
  if (door3State != lastDoor3State) {
  // if the state has changed, increment the counter
    if (door3State == LOW) {
      Serial.println("Door 3 Open");
      client.publish("ha/door/door3","Open");
      digitalWrite(ledPin, HIGH);
          } else {
      Serial.println("Door 3 Closed");
      client.publish("ha/door/door3","Closed");
      digitalWrite(ledPin, LOW);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastDoor3State = door3State;

}
