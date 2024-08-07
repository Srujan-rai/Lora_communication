#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <deque>

#define MAX_MESSAGES 5

const char* ssid = "srujan";
const char* password = "11111111";

AsyncWebServer server(80);
std::deque<String> messages;

void setup() {
  Serial.begin(9600); 
  Serial.println("ESP32 is ready");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

  const char* html = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head><title>LoRa Receiver</title></head>
  <body>
    <h1>Received Messages</h1>
    <div id="messages"></div>
    <script>
      function fetchMessages() {
        fetch('/messages')
          .then(response => response.text())
          .then(data => {
            document.getElementById('messages').innerHTML = data.replace(/\n/g, '<br>');
          });
      }
      setInterval(fetchMessages, 1000); // Fetch messages every second
    </script>
  </body>
  </html>)rawliteral";

  server.on("/", HTTP_GET, [html](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });

  server.on("/messages", HTTP_GET, [](AsyncWebServerRequest *request){
    String messageList;
    for (const String& message : messages) {
      messageList += "Received message: " + message + "\n";
    }
    request->send(200, "text/plain", messageList);
  });

  server.begin();
}

void loop() {
  if (Serial.available()) {
    String receivedMessage = Serial.readStringUntil('\n');
    receivedMessage.trim(); 

    if (receivedMessage.length() > 0) { 
      Serial.print("Received message: ");
      Serial.println(receivedMessage);

      if (messages.size() >= MAX_MESSAGES) {
        messages.pop_front();
      }
      messages.push_back(receivedMessage);
    }
  }
}
