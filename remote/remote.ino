/*
 * ESP8266 WiFi and Voice Remote
 * Harish B
 * Modify rawdata.h with the codes needed for your remote 
 */
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "rawdata.h"

const char* ssid = "ENTER SSID HERE";
const char* password = "ENTER PASSWORD HERE";      

IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4
MDNSResponder mdns;
ESP8266WebServer server(80);

// function for switching/changing channel/volume value.
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// set the specified channel
void set_channel(String value, String *message)
{
   if(message != NULL)
   {
      *message += value;
   }
   for(int i =0; i < value.length(); i++)
    {
      switch(value.charAt(i))
      {
        case '0':
          irsend.sendRaw(rawData0, sizeof(rawData0)/sizeof(uint16_t), 38);
          break;
        case '1':
          irsend.sendRaw(rawData1, sizeof(rawData1)/sizeof(uint16_t), 38);
          break;
        case '2':
          irsend.sendRaw(rawData2, sizeof(rawData2)/sizeof(uint16_t), 38);
          break;
        case '3':
          irsend.sendRaw(rawData3, sizeof(rawData3)/sizeof(uint16_t), 38);
          break;
        case '4':
          irsend.sendRaw(rawData4, sizeof(rawData4)/sizeof(uint16_t), 38);
          break;
        case '5':
          irsend.sendRaw(rawData5, sizeof(rawData5)/sizeof(uint16_t), 38);
          break;
        case '6':
          irsend.sendRaw(rawData6, sizeof(rawData6)/sizeof(uint16_t), 38);
          break;
        case '7':
          irsend.sendRaw(rawData7, sizeof(rawData7)/sizeof(uint16_t), 38);
          break;
        case '8':
          irsend.sendRaw(rawData8, sizeof(rawData8)/sizeof(uint16_t), 38);
          break;
        case '9':
          irsend.sendRaw(rawData9, sizeof(rawData9)/sizeof(uint16_t), 38);
          break;
      }
      delay(100);
    }
    irsend.sendRaw(rawDataCableOk, sizeof(rawDataCableOk)/sizeof(uint16_t), 38); 
}

// root web page
void handle_root() {  
  String message = "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<title>TV Web Remote</title>\n"
  "</head>\n"
  "<body> <h1>Welcome To Smart Remote</h1>\n"
  "<fotter>-Made By <a href='https://github.com/harishb19'>Harish</a> </fotter>"
  "</body>\n"
  "</html>\n";
  server.send(200, "text/html", message);
  delay(100);
}

// button web page
void handle_button() {
  String message = "button send: ";
  String task = "";
  String value = "";
  task="";
  value="";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    if(server.argName(i) == "task")
    {
      task = server.arg(i);
    }
    if(server.argName(i) == "value")
    {
      value = server.arg(i);
    }
  }
  if(task == "tvmute")
  {
     message += "{tvmute}";
     irsend.sendRaw(rawDataMute, sizeof(rawDataMute)/sizeof(uint16_t), 38);   
  }
  else if(task == "cabmute")
  {
     message += "{cabmute}";
     irsend.sendRaw(rawDataCableMute, sizeof(rawDataCableMute)/sizeof(uint16_t), 38);   
  }
  else if(task == "input")
  {
     message += "{input:" + value + "}";
     irsend.sendRaw(rawDataInput, sizeof(rawDataInput)/sizeof(uint16_t), 38);  
     if(value.toInt() > 0)
     {
       delay(1500);
     }
     for(int i = 1; i <= min(10,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataInput, sizeof(rawDataInput)/sizeof(uint16_t), 38);  
       delay(250);
     }
  }
  else if(task == "power")
  {
     message += "{power}";
     irsend.sendRaw(rawDataTVPower, sizeof(rawDataTVPower)/sizeof(uint16_t), 38);  
     delay(100);
     irsend.sendRaw(rawDataCablePower, sizeof(rawDataCablePower)/sizeof(uint16_t), 38);  
  }
  else if(task == "tvpower")
  {
     message += "{tvpower}";
     irsend.sendRaw(rawDataTVPower, sizeof(rawDataTVPower)/sizeof(uint16_t), 38);  
  }
   else if(task == "cabpower")
  {
     message += "{cabpower}";
     irsend.sendRaw(rawDataCablePower, sizeof(rawDataCablePower)/sizeof(uint16_t), 38);  
  }
  else if(task == "tvvolup")
  {
     message += "{tvvolup:" + value + "}";
     for(int i =0; i <= min(29,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataVolumeUp, sizeof(rawDataVolumeUp)/sizeof(uint16_t), 38);  
       delay(100);
     }

  }
  else if(task == "tvvoldn")
  {
     message += "{voldn:" + value + "}";
     for(int i =0; i <= min(29,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataVolumeDown, sizeof(rawDataVolumeDown)/sizeof(uint16_t), 38);  
       delay(100);
     }
  }
   else if(task == "cabvolup")
  {
     message += "{cabvolup:" + value + "}";
     for(int i =0; i <= min(29,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataCableVolumeUp, sizeof(rawDataCableVolumeUp)/sizeof(uint16_t), 38);  
       delay(100);
     }

  }
  else if(task == "cabvoldn")
  {
     message += "{cabvoldn:" + value + "}";
     for(int i =0; i <= min(29,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataCableVolumeDown, sizeof(rawDataCableVolumeDown)/sizeof(uint16_t), 38);  
       delay(100);
     }
  }
  else if(task == "chanup")
  {
     message += "{chanup}";
     for(int i =0; i < min(200,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataChannelUp, sizeof(rawDataChannelUp)/sizeof(uint16_t), 38);  
       delay(100);
     }

  }
  else if(task == "chandn")
  {
     message += "{chandn}";
     for(int i =0; i < min(200,value.toInt()); i++)
     {
       irsend.sendRaw(rawDataChannelDown, sizeof(rawDataChannelDown)/sizeof(uint16_t), 38);  
       delay(100);
     }
  }
  else if(task == "num0")
  {
    message += "{0}";
    irsend.sendRaw(rawData0, sizeof(rawData0)/sizeof(uint16_t),38);
  }
  else if(task == "num1")
  {
    message += "{1}";
    irsend.sendRaw(rawData1, sizeof(rawData1)/sizeof(uint16_t),38);
  }
  else if(task == "num2")
  {
    message += "{2}";
    irsend.sendRaw(rawData2, sizeof(rawData2)/sizeof(uint16_t),38);
  }
  else if(task == "num3")
  {
    message += "{3}";
    irsend.sendRaw(rawData3, sizeof(rawData3)/sizeof(uint16_t),38);
  }
  else if(task == "num4")
  {
    message += "{4}";
    irsend.sendRaw(rawData4, sizeof(rawData4)/sizeof(uint16_t),38);
  }
  else if(task == "num5")
  {
    message += "{5}";
    irsend.sendRaw(rawData5, sizeof(rawData5)/sizeof(uint16_t),38);
  }
  else if(task == "num6")
  {
    message += "{6}";
    irsend.sendRaw(rawData6, sizeof(rawData6)/sizeof(uint16_t),38);
  }
  else if(task == "num7")
  {
    message += "{7}";
    irsend.sendRaw(rawData7, sizeof(rawData7)/sizeof(uint16_t),38);
  }
  else if(task == "num8")
  {
    message += "{8}";
    irsend.sendRaw(rawData8, sizeof(rawData8)/sizeof(uint16_t),38);
  }
  else if(task == "num9")
  {
    message += "{9}";
    irsend.sendRaw(rawData9, sizeof(rawData9)/sizeof(uint16_t),38);
  }
  else if(task == "tvok")
  {
    message += "{tv ok}";
    irsend.sendRaw(rawDataOk, sizeof(rawDataOk)/sizeof(uint16_t),38);
  }
    else if(task == "cabok")
  {
    message += "{cab ok}";
    irsend.sendRaw(rawDataCableOk, sizeof(rawDataCableOk)/sizeof(uint16_t),38);
  }
  else if(task == "tvmenu")
  {
     message += "{tvmenu}";
     irsend.sendRaw(rawDataMenu, sizeof(rawDataMenu)/sizeof(uint16_t), 38);   
  } 
  else if(task == "tvpicture")
  {
     message += "{tvpicture}";
     irsend.sendRaw(rawDataPicture, sizeof(rawDataPicture)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvstill")
  {
     message += "{tvstill}";
     irsend.sendRaw(rawDataStill, sizeof(rawDataStill)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvexit")
  {
     message += "{tvexit}";
     irsend.sendRaw(rawDataExit, sizeof(rawDataExit)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvapp")
  {
     message += "{tvapp}";
     irsend.sendRaw(rawDataApp, sizeof(rawDataApp)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvhome")
  {
     message += "{tvhome}";
     irsend.sendRaw(rawDataHome, sizeof(rawDataHome)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvup")
  {
     message += "{tvup}";
     irsend.sendRaw(rawDataUp, sizeof(rawDataUp)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvdown")
  {
     message += "{tvdown}";
     irsend.sendRaw(rawDataDown, sizeof(rawDataDown)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvleft")
  {
     message += "{tvleft}";
     irsend.sendRaw(rawDataLeft, sizeof(rawDataLeft)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvright")
  {
     message += "{tvright}";
     irsend.sendRaw(rawDataRight, sizeof(rawDataRight)/sizeof(uint16_t), 38);   
  }
   else if(task == "tvreturn")
  {
     message += "{tvreturn}";
     irsend.sendRaw(rawDataReturn, sizeof(rawDataReturn)/sizeof(uint16_t), 38);   
  }
  else if(task == "channel")
  {
    message+= "{channel}";
    set_channel(value, &message);
  }
  else if(task == "random")
  {
    message += "{random}";
    char mem[1024];
    char *tvalue = mem;
    strncpy(mem,value.c_str(),min(1024,sizeof(value)));
    if(strchr(tvalue,'-'))
    {
      const char *range1 = strtok(tvalue,"-");
      const char *range2 = strtok(NULL,"-");
      int newchannel = random(max(atoi(range1),1),min(atoi(range2)+1,5000));
      set_channel(String(newchannel), &message);
    }
    else if(strchr(tvalue,','))
    {
      const char *channels[100];
      int ccount = 0;
      const char *token;
      channels[ccount++] = token = strtok(tvalue,",");
      while(token && ccount < 100)
      {
        channels[ccount++] = token = strtok(NULL,",");
      }
      const char *newchannel = channels[random(0,ccount-1)];
      set_channel(String(newchannel), &message);
    }
  }
  else
  {
      message += " {unknown:" + task + "}";
  }
  Serial.print("button pressed: ");
  Serial.println(task);
  server.send(200, "text/plain", message);
  delay(100);
}

void setup() {

  Serial.println("");
  Serial.println("Smart Remote Web");
  irsend.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);
  server.on("/button", handle_button);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  delay(500);
  server.handleClient();
}
