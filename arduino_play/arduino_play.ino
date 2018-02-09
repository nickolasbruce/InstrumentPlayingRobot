#include <ESP8266WiFi.h>
#include <Servo.h>
#include <string.h>
#include "PhotonicWhistle.h"

int count = 0;

// Wifi settings
const char * SSID = "Linksys00292";
const char * PASSWORD = "1fbjgxdtpv";

WiFiServer server(80);

PhotonicWhistle ins;


void setup() {

  Serial.begin(9600);
  Serial.println("Begin!");
  
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
  ins.begin();
}

void loop() {
  
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
    ins.play(key);

    // respond
    client.flush();
    client.print("HTTP/1.1 200 OK\r\n");
  }
}


