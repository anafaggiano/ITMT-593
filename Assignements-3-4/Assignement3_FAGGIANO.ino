#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>

// Time variables
unsigned long Time ;

//SD card variables
File myFile;

// Ethernet variables
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

//Water conductivity variables

const float ArduinoVoltage = 5.00; // CHANGE THIS FOR 3.3v Arduinos
const float ArduinoResolution = ArduinoVoltage / 1024;

const float resistorValue = 10000.0;
int threshold = 3;

int inputPin = A0;
int ouputPin = A5;
//

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only  
  }

 // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

   //water conductivity setup
 pinMode(ouputPin, OUTPUT);
pinMode(inputPin, INPUT);

//SD card
Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}


 
void loop() {
//Timestamp constants
Time = millis();
//
  

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

//Conductivity constants
returnVoltage = analogValue *ArduinoResolution;
resistance = ((5.00 * resistorValue) / returnVoltage) - resistorValue;  
Siemens = 1.0/(resistance/1000000);
TDS = 500 * (Siemens/1000);
////SD CARD

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    //writing

myFile.print("<tr>");
      myFile.print("<td align='center' valign='middle'>");
      myFile.print(millis());
      myFile.print("</td>");

      myFile.print("<td  align='center' valign='middle'>");
      myFile.print(returnVoltage);
      myFile.print("</td>");

      myFile.print("<td  align='center' valign='middle'>");
      myFile.print(resistance);
      myFile.print("</td>");
      
      myFile.print("<td  align='center' valign='middle'>");
      myFile.print(Siemens);
      myFile.print("</td>");

      myFile.print("<td  align='center' valign='middle'>");
      myFile.print(TDS);
      myFile.print("</td>");
    myFile.print("</td>");

if(returnVoltage>4.9) myFile.println("Are you sure this isn't metal?");

// close the file:
    myFile.close();
    Serial.println("done.");
    delay(5000);
    
    
    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

 // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
  //  Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
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

            //html     

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
              // re-open the file for reading:
              myFile = SD.open("test.txt");
              if (myFile) {
                Serial.println("test.txt:");
            
                // read from the file until there's nothing else in it:
                while (myFile.available()) {
                  char character = myFile.read();
                  client.print(character);
                }
                // close the file:
                Serial.println("done.");
                myFile.close();
              } else {
                // if the file didn't open, print an error:
                Serial.println("error opening test.txt");
              }
            client.println("</table>");
            client.println("</div>");

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
    delay(1000);
    // close the connection:
    client.stop();
  //  Serial.println("client disconnected");
    Ethernet.maintain();
  }
  delay(5000);
}
