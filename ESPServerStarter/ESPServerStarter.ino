#define WIFI_SSD "SHAW-EE9FB0"
#define WIFI_PASSWORD "Sidhu*3175"

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


void setup() {
  // put your setup code here, to run once:
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
  
  //set serial baud to 9600 as that is what is read from by Arduino as serial Input.
  Serial.end();
  Serial.begin(9600);
  //start the server
  server.begin(); 
}

void loop() {
  //handle the request if there is any
  server.handleClient();
}
