#include <ESP8266WiFi.h>
#include <Servo.h>
#include <string.h>

// instrument name
const char * INS_NAME = "PHOTONIC_WHISTLE";


int count = 0;

// pins
Servo pitch;
Servo pluck;
const int PITCH_PIN = D1;
const int PLUCK_PIN = D0;
const int LIGHT_PIN = A0;
const int lightThreshold = 10;
float Rsensor;

// Wifi settings
const char * SSID = "Linksys00292";
const char * PASSWORD = "1fbjgxdtpv";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  
  // setup pins
  pitch.attach(PITCH_PIN);
  pluck.attach(PLUCK_PIN);
  pitch.write(10);
  pluck.write(0);
  
  // login to existing wifi network as station
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }

  // report status
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // update light sensor
  int sensorValue = analogRead(LIGHT_PIN);
  Rsensor = (float)(1023-sensorValue)*10/sensorValue;
  if (Rsensor > lightThreshold) {
    Serial.print("Light taw value: ");
    Serial.println(sensorValue);
    Serial.print("Light resistance: ");
    Serial.println(Rsensor,DEC);
    delay(500);
  }

  
  // check to see if server is still running
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // read first line of request
  String request = client.readStringUntil('\r');
  if (request.length() == 0) {
    return;
  }

  // tokenize the request string to get the key
  char * str = new char [request.length()+1];
  strcpy (str, request.c_str());
  char * pch = strtok(str, "/");

  // skip the first token (GET)
  pch = strtok(NULL, "/");

  // make sure count is different
  int i = 0;
  int new_count = atoi(pch);
  if (count != new_count) {
    count = new_count;

    // finally, get key
    // assume lower case
    pch = strtok(NULL, "/");
    int key = 0;
    if (pch[0] != NULL) {
      key += pch[0]-96;
    }
    if (pch[1] != NULL) {
      key += pch[1]-96;
    }
    
    Serial.print("Key requested: ");
    Serial.println(pch);
    if (INS_NAME == "PHOTONIC_WHISTLE") {
      
    } else {
      play(key);
    }

    // respond
    client.flush();
    client.print("HTTP/1.1 200 OK\r\n");
  }
}

void pluck_it() {
  pluck.write(90);
  delay(1000);
  pluck.write(0);
}

void play(int key) {
  switch (key) {
    case 3:
      // code for low c
      pitch.write(59);
      break;

    case 22:
      // code for c sharp
      break;

    case 4:
      // code for d
      pitch.write(52);
      break;

    case 23:
      // code for d sharp
      break;

    case 5:
      // code for e
      pitch.write(45);
      break;

    case 6:
      // code for f
      pitch.write(38);
      break;

    case 25:
      // code for f sharp
      break;

    case 7:
      // code for g
      pitch.write(31);
      break;

    case 26:
      // code for g sharp
      break;
      
    case 1:
      // code for a
      pitch.write(24);
      break;

    case 20:
      // code for a sharp
      break;

    case 2:
      // code for b
      pitch.write(17);
      break;

    case 11:
      // code for high c
      pitch.write(10);
      break;

    default:
      Serial.println("Invalid input.");
  }
  //pluck_it();
}

