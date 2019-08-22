#define ARDBUFFER 600
#include <stdarg.h>
#include <Arduino.h>

// used to print formatting strings to given serial port
int serialprintf(HardwareSerial &serial, char *str, ...)
{
  int i, count=0, j=0, flag=0;
  char temp[ARDBUFFER+1];
  for(i=0; str[i]!='\0';i++)  if(str[i]=='%')  count++;

  va_list argv;
  va_start(argv, count);
  for(i=0,j=0; str[i]!='\0';i++)
  {
    if(str[i]=='%')
    {
      temp[j] = '\0';
      serial.print(temp);
      j=0;
      temp[0] = '\0';

      switch(str[++i])
      {
        case 'd': serial.print(va_arg(argv, int));
                  break;
        case 'l': serial.print(va_arg(argv, long));
                  break;
        case 'f': serial.print(va_arg(argv, double));
                  break;
        case 'c': serial.print((char)va_arg(argv, int));
                  break;
        case 's': serial.print(va_arg(argv, char *));
                  break;
        default:  ;
      };
    }
    else 
    {
      temp[j] = str[i];
      j = (j+1)%ARDBUFFER;
      if(j==0) 
      {
        temp[ARDBUFFER] = '\0';
        serial.print(temp);
        temp[0]='\0';
      }
    }
  };
  serial.println();
  return count + 1;
}


int isAround360(int degrees){
    if((degrees<5 || degrees>355) )
      return 1;
    else
      return 0;
  }


int getQuadrant(int degrees){
      return degrees/90;  
  
  }

float max4(float x1, float x2, float x3, float x4){
  return max(max(x1,x2),max(x3,x4));
  }
