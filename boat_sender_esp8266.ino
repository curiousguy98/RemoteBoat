#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial swSer(14, 12, false, 256);

// Update these with values suitable for your network.

const char* ssid = "RemoteBoat";
const char* password = "xxxxxxxx";
const char* mqtt_server = "192.168.43.1";///"broker.mqtt-dashboard.com";

const char* SELFIOT_PUBLISH = "boatcontrol";
const char* SELFIOT_SUBSCRIBE = "boatcontrol";
enum __GLOBAL_STATE{
  STATE_IDLE = 0,
  STATE_CONNECTED,
  STATE_START,
} ;

__GLOBAL_STATE g_Running_State = STATE_IDLE;
String connected_json;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char rec_data[256];

  memcpy(rec_data, (char*)payload, length);
  memset(rec_data+length, 0, 1); 
  
  Serial.print(rec_data);
  Serial.println();

  if(rec_data[0]=='1'){
    Serial.print("now turn on left motor");
    digitalWrite(BUILTIN_LED, LOW); 

    swSer.print("A3A3");
	delay(500);
    client.publish(SELFIOT_PUBLISH, "A" );    
	delay(500);
  }
  else if(rec_data[0]=='0'){
    Serial.print("now turn off left motor");
    digitalWrite(BUILTIN_LED, LOW);   

    swSer.print("A5A5");
	delay(500);
    client.publish(SELFIOT_PUBLISH, "B" );    
	delay(500);
  }
  else if(rec_data[0]=='3'){
    Serial.print("now turn on right motor");
    digitalWrite(BUILTIN_LED, LOW);   

    swSer.print("A4A4");
	delay(500);
    client.publish(SELFIOT_PUBLISH, "C" );    
	delay(500);
  }
  else if(rec_data[0]=='2'){
    Serial.print("now turn off right motor");
    digitalWrite(BUILTIN_LED, LOW);   

    swSer.print("A5A5");
	delay(500);
    client.publish(SELFIOT_PUBLISH, "D" );    
	delay(500);
  }
  else if(rec_data[0]=='4'){
    Serial.print("now GO ALL");
    digitalWrite(BUILTIN_LED, LOW);   

    swSer.print("A1A1");
	delay(500);
    client.publish(SELFIOT_PUBLISH, "E" );    
	delay(500);
  }
  else if(rec_data[0]=='5'){
    Serial.print("now STOP ALL");
    digitalWrite(BUILTIN_LED, LOW);   

    swSer.print("A5A5");
	delay(500);
    client.publish(SELFIOT_PUBLISH, "F" );    
	delay(500);
  }
  digitalWrite(BUILTIN_LED, HIGH);


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "selfiotClient-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      g_Running_State = STATE_CONNECTED;
      
      // Once connected, publish an announcement...
      char __connected_json[connected_json.length()+1];
      connected_json.toCharArray(__connected_json, connected_json.length()+1);
      Serial.print(__connected_json);
      ///client.publish(SELFIOT_PUBLISH, __connected_json);
      
      // ... and resubscribe
      client.subscribe(SELFIOT_SUBSCRIBE);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an outputpin
  digitalWrite(BUILTIN_LED, HIGH);
  
  Serial.begin(115200);
  swSer.begin(9600);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);

  g_Running_State = STATE_IDLE;

  StaticJsonBuffer<256> connect_jsonBuffer;
  JsonObject& connect_root = connect_jsonBuffer.createObject();
  connect_root["commandCode"] = "left";
  connect_root["version"] = "0.1";
  
  connect_root.printTo(connected_json);
  
  Serial.println(connected_json);
  
}

void loop() {

  if (!client.connected()) {
    g_Running_State = STATE_IDLE;
    
    reconnect();
  }
  
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

    
  }
}
