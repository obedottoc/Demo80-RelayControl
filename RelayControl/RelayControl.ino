/*
 * This program controls the relay, based on the command 
 * received from the UDP port
 * 
 * 
 * Developed by
 * C. Obedotto
 * Associate Professor,
 * Saveetha Engineering College
 * obedotto@saveetha.ac.in
 * 
 */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/*Relay control pins are connected to D3 and D5 */
#define RELAY_PIN1  D3
#define RELAY_PIN2  D5

const char* ssid     = "SEC WiFi";
const char* password = "";
char dataPacket[255]; 
unsigned int port1 = 4210,port2=4211; 
WiFiUDP Rp1,Rp2;
byte relay1_state=HIGH,relay2_state=HIGH;

/******
 * To setup I/O pins and WiFi connection
 */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(10);
  
  pinMode(RELAY_PIN1,OUTPUT_OPEN_DRAIN); /*To initialize the relay connected pin as output pin*/
  pinMode(RELAY_PIN2,OUTPUT_OPEN_DRAIN);
  
  Serial.begin(115200);
  delay(10);
  digitalWrite(RELAY_PIN1, relay1_state);
  digitalWrite(RELAY_PIN2, relay2_state);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);   
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);    
    delay(500);
  }  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Rp1.begin(port1);
  Rp2.begin(port2);
}

// the loop function runs over and over again forever
void loop() {
  int packetSize;
  int len;

  /*To receive data for controlling relay1*/
  packetSize = Rp1.parsePacket();
  if(packetSize)
  {
    Serial.println("Data received in first port..."); 
    digitalWrite(LED_BUILTIN, LOW); 
    len = Rp1.read(dataPacket, 255);

    /*Turn off the relay if received data is zero*/
    if(dataPacket[len-1]==0)
    {
      relay1_state=LOW;
      digitalWrite(RELAY_PIN1,relay1_state);         
      Serial.println("Switch one off");
    }

    /*Turn on the relay if received data is one*/
    if(dataPacket[len-1]==1)
    {
      relay1_state=HIGH;
      digitalWrite(RELAY_PIN1,relay1_state);
      Serial.println("Switch one on");
    }

    /*Do not change the relay state if the received data is neither zero nor one*/
    if(dataPacket[len-1]==2)
    {
         
    }
    
    dataPacket[0]=relay1_state;
    dataPacket[1]=0;
    Rp1.beginPacket(Rp1.remoteIP(), Rp1.remotePort());
    Rp1.write(dataPacket,1);
    Rp1.endPacket();    
    digitalWrite(LED_BUILTIN, HIGH); 
  }

  /*To receive data for controlling relay2*/
  packetSize = Rp2.parsePacket();
  if(packetSize)
  {
    Serial.println("Data received in second port..."); 
    digitalWrite(LED_BUILTIN, LOW); 
    len = Rp2.read(dataPacket, 255);

    /*Turn off the relay if received data is zero*/
    if(dataPacket[len-1]==0)
    {
      relay2_state=LOW;
      digitalWrite(RELAY_PIN2,relay2_state);         
      Serial.println("Switch two off");
    }

    /*Turn on the relay if received data is one*/
    if(dataPacket[len-1]==1)
    {
      relay2_state=HIGH;
      digitalWrite(RELAY_PIN2,relay2_state);
      Serial.println("Switch two on");
    }

    /*Do not change the relay state if the received data is neither zero nor one*/
    if(dataPacket[len-1]==2)
    {
        
    }
    /*To send the status of the relay to the client*/
    dataPacket[0]=relay2_state;
    dataPacket[1]=0;
    Rp2.beginPacket(Rp2.remoteIP(), Rp2.remotePort());
    Rp2.write(dataPacket,1);
    Rp2.endPacket();
    digitalWrite(LED_BUILTIN, HIGH); 
  }  
}
