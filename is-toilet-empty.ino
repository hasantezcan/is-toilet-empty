      #include <ESP8266WiFi.h>
      #include <WiFiClient.h>
      #include <ESP8266WiFiMulti.h>
      #include <ESP8266mDNS.h>
      #include <ESP8266WebServer.h>
      
      #define lock 13
      
      int wc = 0; // False = toilet is available | true = toilet is not available
      const char* ssid = "Aykut Ş.";
      const char* password = "19981998613";
      
      ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
      
      ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
      
      void handleRoot();              // function prototypes for HTTP handlers
      void handleLED();
      void handleNotFound();
      
      void setup(void){
        pinMode(lock, INPUT);
        Serial.begin(115200);         // Start the Serial communication to send messages to the computer
        delay(10);
        Serial.println('\n');
        wifiMulti.addAP(ssid, password);   // add Wi-Fi networks you want to connect to
      
        Serial.println("Connecting ...");
        int i = 0;
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
      void loop(void){
        wc = digitalRead(lock);
        Serial.println(wc);
        delay(2000);
        server.handleClient();                    // Listen for HTTP requests from clients
      }
      String webPageDolu = R"=====(
      <style media="screen">
        body {
          background-color: red;
        }
        p {
          text-align: center;
          font-size: 15rem;
          color: white;
          margin: auto;
          padding-top: 15rem;
        }
      </style>
       <head>
        <title>LED</title>
        <meta charset=\"utf-8\">
        <title></title>
        </head>
        <body> 
      <p>NOT AVAILABLE</p>
      
      </body>
      </html>
      
       )=====";
       
      String webPageBos = R"=====(
      <style media="screen">
        body {
            background-color: green;
          }
          p {
            text-align: center;
            font-size: 15rem;
            color: white;
            margin: auto;
            padding-top: 15rem;
          }
       </style>
       <head>
        <title>LED</title>
        <meta charset=\"utf-8\">
        <title>Is Toilet Empty?</title>
        </head>
        <body> 
      <p>AVAILABLE</p>
      
      </body>
      </html>
      
       )=====";
      
       
      void handleRoot() {
        if(wc) {
          server.send(200, "text/html", webPageDolu);
        } else {
          server.send(200, "text/html", webPageBos);
        }
      }
      
      void handleNotFound(){
        server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
      }
      
