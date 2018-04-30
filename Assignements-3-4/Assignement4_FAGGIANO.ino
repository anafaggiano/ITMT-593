//Bluetooth config
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

 #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
//End of bluetooth config

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0F, 0x47, 0x79
};
IPAddress ip(192,168,1,31);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

//Water conductivity

const float ArduinoVoltage = 5.00; // CHANGE THIS FOR 3.3v Arduinos
const float ArduinoResolution = ArduinoVoltage / 1024;

const float resistorValue = 10000.0;
int threshold = 3;

int inputPin = A0;
int ouputPin = A5;

void setup() {

  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only}*/
//BLUETOOTH SETUP

while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
   Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }
//END OF BLUETOOTH SETUP
  
 //water conductivity
 pinMode(ouputPin, OUTPUT);
pinMode(inputPin, INPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
 // Serial.print("server is at ");
 // Serial.println(Ethernet.localIP());
}


void loop() {
  //Water conductivity constants
  int analogValue=0;
int oldAnalogValue=1000;
float returnVoltage=0.0;
float resistance=0.0;
double Siemens;
float TDS=0.0;

//Conductivity loop
while(((oldAnalogValue-analogValue)>threshold) || (oldAnalogValue<50))
{
 oldAnalogValue = analogValue;
 digitalWrite( ouputPin, HIGH );
 delay(10); // allow ringing to stop
 analogValue = analogRead( inputPin );
 digitalWrite( ouputPin, LOW );
}
//

//Conductivity in console

returnVoltage = analogValue *ArduinoResolution;  
resistance = ((5.00 * resistorValue) / returnVoltage) - resistorValue;
Siemens = 1.0/(resistance/1000000);
TDS = 500 * (Siemens/1000);


//delay(500);
// listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
 //   Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
    //    Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          
          // Conductivity HTTP
          client.println("<div class='container'>");
            client.println("<p>My Data</p>");

             client.println("<table >");
              client.println("<tr>");
                client.println("<th>Milliseconds (ms)</th>");
                client.println("<th>Voltage (V)</th>");
                client.println("<th>Resistance (Ohm)</th>");
                client.println("<th>Conductivity (Siemens)</th>");
                client.println("<th>TDS (PPM)</th>");
             
              client.println("</tr>");
              
        client.print("<tr>");
      client.print("<td align='center' valign='middle'>");
      client.print(millis());
      client.print("</td>");

      client.print("<td  align='center' valign='middle'>");
      client.print(returnVoltage);
      client.print("</td>");

      client.print("<td  align='center' valign='middle'>");
      client.print(resistance);
      client.print("</td>");
      
      client.print("<td  align='center' valign='middle'>");
      client.print(Siemens);
      client.print("</td>");

      client.print("<td  align='center' valign='middle'>");
      client.print(TDS);
      client.print("</td>");
    client.print("</td>");

if(returnVoltage>4.9) client.println("Are you sure this isn't metal?");
String voltage=String(returnVoltage);
Serial.print(voltage);
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    //delay(1000);
    // close the connection:
    client.stop();
    //Serial.println("client disconnected");
    
  }

  ble.print(F("AT+BLEUARTTX="));
  ble.print(F(" Voltage = ")); 
  ble.print(returnVoltage);
  ble.print(F(" V ")); 

  ble.print(F(" Resistance = "));
  ble.print(resistance);
   ble.print(F(" Ohms "));
 
  ble.print(F(" Conductivity = "));
  ble.print(Siemens);
   ble.print(F(" Siemens ")); 
  
  ble.print(F(" TDS = "));
  ble.print(TDS);
   ble.print(F(" PPM ")); 
 
  ble.println("               *****            ");
  delay(5000);
}
