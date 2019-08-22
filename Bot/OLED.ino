#include "Aroid_Logo.h"
#define LOGO_SCREEN 0
#define INFO_SCREEN 1

unsigned long lastScreenChange = -5000;
unsigned long currentMillis=0;
int currentScreen=INFO_SCREEN;

// Makes Ariod switch between Aroid logo and Network Info on the OLED
void displayContentOnScreen(){
  
  currentMillis = millis();

  if(currentScreen==LOGO_SCREEN && currentMillis-lastScreenChange>5000){
    printNetworkInfo();
    currentScreen=INFO_SCREEN;
    lastScreenChange = currentMillis;
    return;
  }
  
  if(currentScreen==INFO_SCREEN && currentMillis-lastScreenChange>10000){
    drawLogo();
    currentScreen=LOGO_SCREEN;
    lastScreenChange = currentMillis;
    return;
  }
  
}
// Draws Aroid logo on the screen
void drawLogo() {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp_black_on_white, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display(); 
}
// function prints network information on to the OLED
void printNetworkInfo(){
      display.clearDisplay();
      display.setTextSize (1.2);
      display.setTextColor (WHITE);
      display.setCursor (0,10);
      display.println ("Aroid IP:" + (String) ARDUINO_IP); 
      display.setCursor (0,25); 
      display.println ("Cam IP:"+(String) CAMERA_IP); 
      display.setCursor (0,40); 
      display.println ("Gateway:"+(String) NETWORK_GATEWAY); 
      display.setCursor (0,55); 
      display.println ("Subnet:"+(String) SUBNET); 
      
      display.display();
  
  }
