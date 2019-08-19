#define WIFI_SSD "********"
#define WIFI_PASSWORD "*********"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//IP address that will be assigned to the ESP Server
IPAddress staticIP(192, 168, 0, 99); 
//Gateway address to the network
IPAddress gateway(192, 168, 0, 1); 
//Subnet mask  
IPAddress subnet(255, 255, 255, 0); 
//DNS 
IPAddress dns(8, 8, 8, 8);  

//Create a server object that listens on port 80
ESP8266WebServer server(80); 

// handles request to the webroot
// it just sends a simple message in response that validates that the server is running
void handleRoot() {
 server.send(200, "text/html", "Welcome to the ESP8266!!");
}

// API end point that can be used to forward serial Input to the Arduino
void handleSerialInput(){
  // if there is no content to be forwarded to the Arduino then return status code 400
   if( ! server.hasArg("input") || server.arg("input") == NULL) { 
        server.send(400, "text/plain", "400: Invalid Request");
        return;
    }
    // if there is an input in the body it would write it to the Serial output that would be read by the Arduino
    else{
      Serial.println(server.arg("input"));
      server.send(200, "text/plain", "Serial Input successfully written to Serial3 of Arduino!");
      return;
    }
   
  }

 // API end point that can be used to read serial Input to the Arduino
void handleSensorRead(){
  // if there is no content to be forwarded to the Arduino then return status code 400
   if( ! server.hasArg("sensorType") || server.arg("sensorType") == NULL) { 
        server.send(400, "text/plain", "400: Invalid Request");
        return;
    }
    // if there is an input in the body it would write it to the Serial output that would be read by the Arduino
    else{
      Serial.println(server.arg("sensorType"));
      bool timeOut = 0;
      int  waitDurationMill = 5000;
      
      unsigned long startTime = millis();
      while(Serial.available()==0 && ! timeOut){
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - startTime;
        if(elapsedTime>waitDurationMill){
            timeOut=1;
            break;
          }
      }
      if(!timeOut){
        server.send(200, "text/plain", Serial.readStringUntil('\n'));
        return;
      }
      else{
        server.send(408, "text/plain", "Request timed out!");
        return;
      }
    }
   
  }


void setup() {
  // put your setup code here, to run once: 115200
  Serial.begin(115200);
  Serial.println();

  //Attempt connection to WiFi
  Serial.printf("Connecting to the WiFi: %s\n", WIFI_SSD);
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(WIFI_SSD, WIFI_PASSWORD);
  //wait until connection is successful
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //print Local IP of ESP8266
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //set handlers for different API endpoints
  server.on("/", handleRoot);
  server.on("/serialInput", HTTP_POST, handleSerialInput); 
  server.on("/sensorRead", HTTP_POST, handleSensorRead); 
  
  //set serial baud to 9600 as that is what is read from by Arduino as serial Input.
  Serial.end();
  Serial.begin(9600);
  
  Serial.println("WC,");
  //start the server
  server.begin(); 
}

void loop() {
  //handle the request if there is any
  server.handleClient();
}
