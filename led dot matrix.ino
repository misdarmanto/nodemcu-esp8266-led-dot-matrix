#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 4
#define DATA_PIN 14
#define CS_PIN 12
#define CLK_PIN 13

String displayDefault = "Hello World";

ESP8266WebServer server(80);
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void handleRoot () {
  const char buffer [] = R"=====(
      <!DOCTYPE html>
      <html lang="en">
      <head>
          <meta charset="UTF-8">
          <meta http-equiv="X-UA-Compatible" content="IE=edge">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <title>LED</title>
          <style>
              body{
                  display: flex;
                  justify-content: center;
                  align-items: center;
                  margin-top: 100px;         
              }
              input{
                  width: 400px;
                  padding: 20px;
                  border-radius: 10px;
                  border-style: none;
                  border: 1px solid gray;
                  margin-right: 20px; 
              }
              button{
                  border: none;
                  border-radius: 10px;
                  background-color: rgb(255, 30, 255);
                  color: white;
                  font-size: 18px;
                  padding: 10px 20px;
              }
          </style>
      </head>
      <body>
          <input type="text" id="input">
          <button onclick="sendData()" >Send</button>
          <script>
              function sendData() {
                  const inputData = document.querySelector("#input");
                  const xhttp = new XMLHttpRequest();
                  xhttp.onreadystatechange = function() {
                      if (this.readyState == 4 && this.status == 200) {
                          console.log(xhttp.statusText);
                      }
                  };
                  xhttp.open("GET", "handleUser?message="+inputData.value, true);
                  xhttp.send();
                  inputData.value = "";
              }
          </script>
      </body>
      </html>
    )=====";

    server.send(200, "text/html", buffer);
}

void handleMassage(){
  String msg = server.arg("message");
  displayDefault = msg;
  server.send(200, "text/plain", "ok");
}

void setup() {
    
  myDisplay.begin();
  myDisplay.setIntensity(0);
  myDisplay.displayClear();
  
  WiFi.begin("fakir_bandwith", ".12348765");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  server.on("/", handleRoot);
  server.on("/handleUser", handleMassage);
  server.begin();
}

void loop() {
   server.handleClient();
   myDisplay.setTextAlignment(PA_CENTER);
   myDisplay.print(displayDefault);
}