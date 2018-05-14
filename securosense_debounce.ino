#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// this constant won't change:
const int door1Pin = D1;   // the pin that the door is attached to
const int door2Pin = D2;    // the pin that door 2 is attached to     
const int door3Pin = D3;    // the pin that door 3 is attached to
const int window1Pin = D5;  // the pin that window 1 is attached to
const int window2Pin = D6;  // the pin that window 2 is attached to
const int ledPin = D4;      // the pin that the LED is attached to

// Current State:
int door1State = 0; 
int door2State = 0;
int door3State = 0;
int window1State = 0;
int window2State = 0;
//Last State
int lastDoor1State = 0;     
int lastDoor2State = 0;
int lastDoor3State = 0;
int lastWindow1State = 0;
int lastWindow2State = 0;

//Debounce
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;



//WiFi & MQTT 
const char* ssid = "xxx";
const char* password = "xxx";
const char* mqtt_server = "xxx";
const char* mqtt_server_user = "xxx";
const char* mqtt_server_pass = "xxx";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  // initialize the button pin as a input:
  pinMode(door1Pin, INPUT_PULLUP);
  pinMode(door2Pin, INPUT_PULLUP);
  pinMode(door3Pin, INPUT_PULLUP);
  pinMode(window1Pin, INPUT_PULLUP);
  pinMode(window2Pin, INPUT_PULLUP);
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
  door1State = digitalRead(door1Pin);
  door2State = digitalRead(door2Pin);
  door3State = digitalRead(door3Pin);
  window1State = digitalRead(window1Pin);
  window2State = digitalRead(window2Pin);
  //Door 1 = Front Door
  //Door 2 = Back Door
  //Door 3 = Garage Door
  //Window 1 =
  //Window 2 =
  // compare the door to its previous state
    if (door1State != lastDoor1State) {
      lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the door state has changed
    if (door1State == HIGH) {
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
   lastDoor1State = door1State;
  //##### Door2 #######
    if (door2State != lastDoor2State) {
      lastDebounceTime1 = millis();
    }
    
    if ((millis() - lastDebounceTime1) > debounceDelay) {
    // if the door state has changed
    if (door2State == HIGH) {
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
      lastDebounceTime2 = millis();
    }
    
    if ((millis() - lastDebounceTime2) > debounceDelay) {
  // if the state has changed, increment the counter
    if (door3State == HIGH) {
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
    //##### Window 1 ######
    if (window1State != lastWindow1State) {
      lastDebounceTime3 = millis();
    }
    
    if ((millis() - lastDebounceTime3) > debounceDelay) {
  // if the state has changed, increment the counter
    if (window1State == HIGH) {
      Serial.println("Window 1 Open");
      client.publish("ha/window/window1","Open");
      digitalWrite(ledPin, HIGH);
          } else {
      Serial.println("Door 3 Closed");
      client.publish("ha/window/window1","Closed");
      digitalWrite(ledPin, LOW);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastWindow1State = window1State;
  
    //##### Window 2 ######
 if (window2State != lastWindow2State) {
      lastDebounceTime4 = millis();
    }
    
    if ((millis() - lastDebounceTime4) > debounceDelay) {
  // if the state has changed, increment the counter
    if (window2State == HIGH) {
      Serial.println("Window 2 Open");
      client.publish("ha/window/window2","Open");
      digitalWrite(ledPin, HIGH);
          } else {
      Serial.println("Window 2 Closed");
      client.publish("ha/window/window2","Closed");
      digitalWrite(ledPin, LOW);
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastWindow2State = window2State;
}
