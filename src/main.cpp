#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>

int IN1 = 26;
int IN2 = 27;
int velocidadeA = 25;

int IN3 = 14;
int IN4 = 12;
int velocidadeB = 13;

const char* ssid = "";
const char* password = "";

const int motor1PWM = 25;
const int motor1Dir1 = 26;
const int motor1Dir2 = 27;

const int motor2PWM = 13;
const int motor2Dir1 = 14;
const int motor2Dir2 = 12;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<style>
  body {
    margin: 0;
    display: flex;
    justify-content: center;
    align-items: center;
    background-color: #f0f0f0;
  }

  .container {
    position: relative;
    width: 90%;
    max-width: 700px;
    height: 400px;
    margin-top: 20px;
    background-color: #ffffff;
    border: 1px solid rgb(51, 51, 51);
    border-radius: 10px;
    overflow: hidden;
  }

  .header {
    text-align: center;
    padding: 10px;
    height: 5%;
    background-color: #ff2828;
    color: #000000;
  }

  .circle-container {
    display: flex;
    justify-content: space-between;
    align-items: center;
    width: 90%;
    height: 95%;
    margin: auto;
  }

  .outer-circle {
    width: 250px;
    height: 250px;
    background-color: #a8a8a8;
    border-radius: 50%;
    display: flex;
    justify-content: center;
    align-items: center;
    position: relative;
  }

  .inner-circle {
    width: 50px;
    height: 50px;
    background-color: #63ff25;
    border-radius: 50%;
    position: absolute;
    box-shadow: 0 0 20px rgba(0, 0, 0, 0.9);

  }

</style>
</head>
<body>
  <div class="container">
    <div class="header" id="header">
        Conexão OK
    </div>
    <div class="circle-container">
      <div class="outer-circle" id="outerCircle1">
        <div class="inner-circle" id="innerCircle1"></div>
      </div>
      <div class="outer-circle" id="outerCircle2">
        <div class="inner-circle" id="innerCircle2"></div>
      </div>
    </div>
  </div>

  <script>
    const innerCircle1 = document.getElementById("innerCircle1");
    const innerCircle2 = document.getElementById("innerCircle2");
    const outerCircle1 = document.getElementById("outerCircle1");
    const outerCircle2 = document.getElementById("outerCircle2");

    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    window.addEventListener('load', onLoad);

    function initWebSocket() {
        console.log('Trying to open a WebSocket connection...');
        document.getElementById("header").innerHTML = 'Trying to connect to ' + window.location.hostname + '...'
        websocket = new WebSocket(gateway);
        websocket.onopen = onOpen;
        websocket.onclose = onClose;
    }

    function onOpen(event) {
        console.log('Connection opened');
        document.getElementById("header").innerHTML = 'Connection OK'
        document.getElementById("header").style.backgroundColor = '#48ff00'
    }

    function onClose(event) {
        console.log('Connection closed');
        document.getElementById("header").innerHTML = 'Connection fail'
        document.getElementById("header").style.backgroundColor = '#ff2828'
        setTimeout(initWebSocket, 2000);
    }

    function onLoad(event) {
        initWebSocket();
    }

    function send_msg(key, value) {
      temp = key + ' = ' + value
      // console.log(temp);
      websocket.send(temp);
    }

    function onStartX(event) {
      event.preventDefault();

      const circle = innerCircle2;
      const initialX = (event.type === "touchstart") ? event.touches[0].clientX : event.clientX;
      const circleRect = circle.getBoundingClientRect();
      const circleOffsetX = initialX - circleRect.left;
      const containerRect = circle.parentElement.getBoundingClientRect();
 
      document.addEventListener((event.type === "touchstart") ? "touchmove" : "mousemove", onMove);
      document.addEventListener((event.type === "touchstart") ? "touchend" : "mouseup", onEnd);

      function onMove(event) {
        const newX = ((event.type === "touchmove") ? event.touches[0].clientX : event.clientX) - containerRect.left - circleOffsetX;
        const maxX = containerRect.width - circleRect.width;
        const clampedX = Math.min(Math.max(0, newX), maxX);
        circle.style.left = clampedX + "px";
        if(newX > 0 && newX < maxX) {
          send_msg('direction', parseInt(100 - newX));
        }
      }

      function onEnd() {
        document.removeEventListener((event.type === "touchstart") ? "touchmove" : "mousemove", onMove);
        document.removeEventListener((event.type === "touchstart") ? "touchend" : "mouseup", onEnd);
        innerCircle2.style.left = "100px";
        send_msg('direction', 0)
      }
    }

    function onStartY(event) {
      event.preventDefault();

      const circle = innerCircle1;
      const initialY = (event.type === "touchstart") ? event.touches[0].clientY : event.clientY;
      const circleRect = circle.getBoundingClientRect();
      const circleOffsetY = initialY - circleRect.top;
      const containerRect = circle.parentElement.getBoundingClientRect();

      document.addEventListener((event.type === "touchstart") ? "touchmove" : "mousemove", onMove);
      document.addEventListener((event.type === "touchstart") ? "touchend" : "mouseup", onEnd);

      function onMove(event) {
        const newY = ((event.type === "touchmove") ? event.touches[0].clientY : event.clientY) - containerRect.top - circleOffsetY;
        const maxY = containerRect.height - circleRect.height;
        const clampedY = Math.min(Math.max(0, newY), maxY);      
        circle.style.top = clampedY + "px";
        if(newY > 0 && newY < maxY) {
          send_msg('acelerator', parseInt(100 - newY));
        }
      }

      function onEnd() {
        document.removeEventListener((event.type === "touchstart") ? "touchmove" : "mousemove", onMove);
        document.removeEventListener((event.type === "touchstart") ? "touchend" : "mouseup", onEnd);
        innerCircle1.style.top = "100px";
        send_msg('acelerator', 0);
      }
    }

    outerCircle1.addEventListener("mousedown", onStartY);
    outerCircle1.addEventListener("touchstart", onStartY);

    outerCircle2.addEventListener("mousedown", onStartX);
    outerCircle2.addEventListener("touchstart", onStartX);
  </script>
</body>
</html>
)rawliteral";

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        char *token;
        const char delimiter = '=';
        int value = 0;
        char copy[16];

        strncpy(copy, (const char*)data, len);
        token = strchr(copy, delimiter);
        value = atoi(token+2);

        *token = '\0';
        // printf("data = |%s|, value = %d\n", copy, value);

        if(strcmp(copy, "direction ") == 0){
            if(value > 0){
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                analogWrite(velocidadeA, value*2);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                analogWrite(velocidadeB, value*2);
            } else if(value < 0) {
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, LOW);
                analogWrite(velocidadeA, value*-2);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                analogWrite(velocidadeB, value*-2);
            } else {
                analogWrite(velocidadeA, 0);
                analogWrite(velocidadeB, 0);
            }

        } else if(strcmp(copy, "acelerator ") == 0) {
            if(value > 0){
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, LOW);
                analogWrite(velocidadeA, value*2);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                analogWrite(velocidadeB, value*2);
            } else if(value < 0) {
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                analogWrite(velocidadeA, value*-2);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                analogWrite(velocidadeB, value*-2);
            } else {
                analogWrite(velocidadeA, 0);
                analogWrite(velocidadeB, 0);
            }
        }
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
    Serial.begin(115200);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(velocidadeA, OUTPUT);
    pinMode(velocidadeB, OUTPUT);

    analogWrite(velocidadeA, 0);
    analogWrite(velocidadeB, 0);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());

    initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html);
    });

    server.begin();
}

void loop() {
    ws.cleanupClients();
}
