#include <ESP8266WiFi.h>
#include <Servo.h>
#include <stdio.h>
#include <string.h>


int count = 0;
Servo pitch_motor;

// pins
int SERVO_PIN = 1;
int pos = 0;


WiFiServer server(80);
void setup() {
  // setup pins
  pitch_motor.attach(SERVO_PIN);
  pitch_motor.write(0);
  
  // create access point with login
  WiFi.mode(WIFI_AP);
  WiFi.softAP("WhyFi", "password");
  server.begin();

  // static ip, gateway, netmask
  WiFi.config(IPAddress(192,168,1,2), 
              IPAddress(192,168,1,1), 
              IPAddress(255,255,255,0));

  // get server IP
  Serial.begin(9600);
  IPAddress HTTPS_ServerIP = WiFi.softAPIP();
  Serial.print("Server IP is: ");
  Serial.println(HTTPS_ServerIP);
}

void loop() {
  // check to see if server is still running
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("Somebody has connected.");

  // read first line of request
  String request = client.readStringUntil('\r');
  if (!request.length()) {
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
      key += pch[0]-97;
    }
    if (pch[1] != NULL) {
      key += pch[1]-97;
    }
    play(key);

    // respond
    client.flush();
    client.print("HTTP/1.1 200 OK\r\n");
  }
}

void play(int key) {
  switch (key) {
    case 3:
      // code for low c
      pitch_motor.write(45);
      break;

    case 22:
      // code for c sharp
      pitch_motor.write(135);
      break;

    case 4:
      // code for d
      break;

    case 23:
      // code for d sharp
      break;

    case 5:
      // code for e
      break;

    case 6:
      // code for f
      break;

    case 25:
      // code for f sharp
      break;

    case 7:
      // code for g
      break;

    case 26:
      // code for g sharp
      break;
      
    case 1:
      // code for a
      break;

    case 20:
      // code for a sharp
      break;

    case 2:
      // code for b
      break;

    case 11:
      // code for high c
      break;

    default:
      Serial.println("Invalid input.");
  }
}

