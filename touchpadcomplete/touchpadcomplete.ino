//la libraire WiFiUdp est dispo dans le repo github arduino ainsi que des examples
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
char ssid[] = "localpi"; //  your network SSID (name) 
char pass[] = "";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on
IPAddress piIP(192,168,1,44);
unsigned int piPORT = 33333;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet
char  ReplyBuffer[] = "stop";       // a string to send back
char  ReplyStart[] = "start";       // a start string
char  ReplyReset[] = "reset";       // a reset string


WiFiUDP Udp;

int flexiForcePin = A0; //analog pin 0

const int LED = 8;  // pin for LED
const int BUTTON = 5; // start button input where pushbutton is connected
const int BUTTONRESET = 2;  // reset button

int val = 0;  // store state variable
int valreset = 0; // set reset button to zero on start

int old_val = 0;  //store previous value

int state = 0;  // 0 on and 1 off for LED

void setup(){
  Serial.begin(9600);
  pinMode(LED, OUTPUT);  // tell LED is output
  pinMode(BUTTON, INPUT); // tell button input
  pinMode(BUTTONRESET, INPUT); // tell reset input botton
  
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);
  
    // wait 10 seconds for connection:
    delay(1000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);  
  
}

void loop(){
  
  val = digitalRead(BUTTON);  // read input value
  valreset = digitalRead(BUTTONRESET); // read value of reset button
  int flexiForceReading = analogRead(flexiForcePin); 
  
  if(flexiForceReading > 10 )
  {

  Serial.println(flexiForceReading);
  
      // send a reply, to the IP address and port that sent us the packet we received
    
    Udp.beginPacket(piIP, piPORT);
    //Serial.print(jam);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    
    //digitalWrite(4, LOW);
   //delay(10000);
  
  delay(2000); //just here to slow down the output for easier reading
  }

  if(val == HIGH )
  {
   
    Udp.beginPacket(piIP, piPORT);
    //Serial.print(jam);
    Udp.write(ReplyStart);
    Udp.endPacket();
    
  delay(1000); //just here to slow down the output for easier reading
    
   state = 1;
     digitalWrite(LED, HIGH); 
   //Serial.println(1); 
     if(state == 1)
    {
     Serial.println(1);
       delay(600); 


    } 
    
    
  }
    
  else if(valreset == HIGH)
  {
    
    Udp.beginPacket(piIP, piPORT);
    //Serial.print(jam);
    Udp.write(ReplyReset);
    Udp.endPacket();
    
  delay(1000); //just here to slow down the output for easier reading
    
    state = 2;
     digitalWrite(LED, HIGH); 
   //Serial.println(1); 
     if(state == 2)
    {
     Serial.println(2);
       delay(600); 


    } 
    
  }
   
  else
  {
   digitalWrite(LED, LOW);
     state = 0; 
   //Serial.println(0);  
  }
 
  
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
