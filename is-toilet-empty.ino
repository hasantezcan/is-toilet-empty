#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <SPI.h>

#define lock1 15
#define lock2 13
#define lock3 12
#define lock4 14

int wc1 = 0; // 0 = toilet is available | 1 = toilet is not available
int wc2 = 0; // 0 = toilet is available | 1 = toilet is not available
int wc3 = 0; // 0 = toilet is available | 1 = toilet is not available
int wc4 = 0; // 0 = toilet is available | 1 = toilet is not available
String page = "";
const char* ssid = "ERYILMAZ";
const char* password = "eryilmaz96";
const String HTTP_TOP = "<html> <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\"> <style> .title{ text-align: center; font-size: 10rem;} .available { background-color: green; color: white; font-size: 10rem; text-align: center; border: .2rem solid white } .notavailable { background-color: red; color: white; font-size: 10rem; text-align: center; border: .2rem solid white } .avb { content: \"AVAILABLE\" } .notavb { content: \"NOT AVAILABLE\" } </style> <head> <title>LED</title> <meta http-equiv=\"refresh\" content=\"2\"> <meta charset=\"utf - 8\"> <title>Is Toilet Empty?</title> </head> <body> <p class=\"title\" >Is Toilet Empty</p> <div class=\"row\"> <div class=\"row mt-5\"/>  <div class=\"row\">";
const String DIV = "<div class=\"col available\">";
const String DIV_NOT = "<div class=\"col notavailable\">";
const String P_TAG1 = "<p>WC-1</p>";
const String P_TAG2 = "<p>WC-2</p>";
const String P_TAG3 = "<p>WC-3</p>";
const String P_TAG4 = "<p>WC-4</p>";
const String P_AVAILABLE = "<p>AVAILABLE</p>";
const String P_NOT_AVAILABLE = "<p>NOT AVAILABLE</p>";
const String CLOSE_DIV = "</div>";
const String HTTP_BOTTOM = "</div></div></body></html>";

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleLED();
void handleNotFound();

void setup(void) {
  pinMode(lock1, INPUT);
  pinMode(lock2, INPUT);
  pinMode(lock3, INPUT);
  pinMode(lock4, INPUT);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  wifiMulti.addAP(ssid, password);   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  page = ""; 
  wc1 = digitalRead(lock1);
  wc2 = digitalRead(lock2);
  wc3 = digitalRead(lock3);
  wc4 = digitalRead(lock4);
  check();
  server.handleClient();                    // Listen for HTTP requests from clients
}


void check() {
  page += HTTP_TOP;
  if (wc1 == 1) {
    page += DIV_NOT;
    page += P_TAG1;
    page += P_NOT_AVAILABLE;
    page += CLOSE_DIV;
  } else {
    page += DIV;
    page += P_TAG1;
    page += P_AVAILABLE;
    page += CLOSE_DIV;
  }
  if (wc2 == 1) {
    page += DIV_NOT;
    page += P_TAG2;
    page += P_NOT_AVAILABLE;
    page += CLOSE_DIV;
  } else {
    page += DIV;
    page += P_TAG2;
    page += P_AVAILABLE;
    page += CLOSE_DIV;
  }
  if (wc3 == 1) {
    page += DIV_NOT;
    page += P_TAG3;
    page += P_NOT_AVAILABLE;
    page += CLOSE_DIV;
  } else {
    page += DIV;
    page += P_TAG3;
    page += P_AVAILABLE;
    page += CLOSE_DIV;
  }
  if (wc4 == 1) {
    page += DIV_NOT;
    page += P_TAG4;
    page += P_NOT_AVAILABLE;
    page += CLOSE_DIV;
  } else {
    page += DIV;
    page += P_TAG4;
    page += P_AVAILABLE;
    page += CLOSE_DIV;
  }
  page += HTTP_BOTTOM;
}


void handleRoot() {
  server.send(200, "text/html", page);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
