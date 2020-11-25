#include <ArduinoJson.h>

#include <ArduinoJson.h>

#include <ArduinoJson.h>
#include <AXP192.h>
#include <IMU.h>
#include <M5Display.h>
#include <M5StickC.h>
#include <RTC.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

HardwareSerial mySerial(2);


const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

#ifndef STASSID
#define STASSID "ADB-CFF9A1"
#define STAPSK  "rce6bn743cjr"
//#define STASSID "Wi-Pi"
//#define STAPSK "brg18f12"

#endif

const char *ssid = STASSID;
const char *password = STAPSK;

WebServer server(80);

const int led = 10;

void setup()
{
    M5.begin();
    Serial.begin(115200);
    mySerial.begin(19200, SERIAL_8N1, 36, 26);
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);

    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    //M5.Lcd.fillScreen(BLACK);
    //delay(100);
    //M5.Lcd.setCursor(0, 10);
    //M5.Lcd.setTextColor(WHITE);
    //M5.Lcd.setTextSize(2);
    //const char* IP_addr = WiFi.localIP();
   // M5.Lcd.printf();

    if (MDNS.begin("esp8266"))
    {
        Serial.println("MDNS responder started");
    }

    server.on("/json",json_test);

    server.on("/", handleRoot);

    server.on("/postplain/", handlePlain);

    server.on("/postform/", handleForm);

    server.on("/led_on/", led_on);

    server.on("/led_off/", led_off);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
  /*
    M5.Lcd.fillScreen(BLACK);
    delay(100);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.printf("M5 Test lcd!");
    Serial.println("M5 Test uart!"); */
    ;
}

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Napis sem volaco! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
      <input type=\"text\" name=\'{\"Command\": \"\' value=\''><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>LED ON! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/led_on/\">\
      <input type=\"submit\" value=\"LED ON\">\
    </form>\
    <h1>LED OFF! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/led_off/\">\
      <input type=\"submit\" value=\"LED OFF\">\
    </form>\
  </body>\
</html>";


const String LED_ON_HTML = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Napis sem volaco! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
      <input type=\"text\" name=\'{\"Command\": \"\' value=\''><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>LED ON! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/led_on/\">\
      <input type=\"submit\" value=\"LED ON\">\
    </form>\
    <h1>LED OFF! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/led_off/\">\
      <input type=\"submit\" value=\"LED OFF\">\
    </form>\
  </body>\
</html>";



const String LED_OFF_HTML = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccc00; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Napis sem volaco! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
      <input type=\"text\" name=\'{\"Command\": \"\' value=\''><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>LED ON! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/led_on/\">\
      <input type=\"submit\" value=\"LED ON\">\
    </form>\
    <h1>LED OFF! </h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/led_off/\">\
      <input type=\"submit\" value=\"LED OFF\">\
    </form>\
  </body>\
</html>";



void json_test()
{
 if (server.method() != HTTP_GET)
    {
        server.send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        StaticJsonDocument<200> Buffer;
        //JsonObject& root = jsonBuffer.createObject();   // struktura
        Buffer["ADC"] = "ADC1";

        JsonArray data = Buffer.createNestedArray("data"); // vnorena struktura
          data.add(11);                 // jednotlive bajty prosceeee 
          data.add(22); 
          data.add(33); 

        JsonArray data_2 = Buffer.createNestedArray("data_2");
          data_2.add(44);                 
          data_2.add(55); 
          data_2.add(66); 
        //Buffer.printTo(Serial);

        String json;
        serializeJson(Buffer, json);          //naondim to na json formatik


        

        //digitalWrite(led, 1);
        server.send(200, "application/json", json);  //proscceeee
        Serial.println("[POST] Led ON");
        digitalWrite(led, 0);
        mySerial.println("CELL ON");
    }
}


void handleRoot()
{
    // digitalWrite(led, 1);
    server.send(200, "text/html", postForms);
    // digitalWrite(led, 0);
}


void led_on()
{
    if (server.method() != HTTP_POST)
    {
        server.send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        //digitalWrite(led, 1);
        server.send(200, "text/html", LED_ON_HTML);
        Serial.println("[POST] Led ON");
        digitalWrite(led, 0);
        mySerial.println("CELL ON");
    }
}

void led_off()
{
    if (server.method() != HTTP_POST)
    {
        server.send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        //digitalWrite(led, 1);
        server.send(200, "text/html", LED_OFF_HTML);
        Serial.println("[POST] Led OFF");
        digitalWrite(led, 1);
        mySerial.println("CELL OFF");
    }
}


void handlePlain()
{
    if (server.method() != HTTP_POST)
    {
        // digitalWrite(led, 1);
        server.send(405, "text/plain", "Method Not Allowed");
        // digitalWrite(led, 0);
    }
    else
    {
        //  digitalWrite(led, 1);
        server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
        Serial.println("POSTXXX body was:\n" + server.arg("plain"));
        //  digitalWrite(led, 0);
    }
}

void handleForm()
{
    if (server.method() != HTTP_POST)
    {
        //digitalWrite(led, 1);
        server.send(405, "text/plain", "Method Not Allowed");
        //digitalWrite(led, 0);
    }
    else
    {
        // digitalWrite(led, 1);
        String message = "POST form was:\n";
        for (uint8_t i = 0; i < server.args(); i++)
        {
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        }
        server.send(200, "text/plain", message);
        Serial.println(message);
        // digitalWrite(led, 0);
    }
}

void handleNotFound()
{
    digitalWrite(led, 1);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    //digitalWrite(led, 0);
}

void loop(void)
{
    server.handleClient();
}