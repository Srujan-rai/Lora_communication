#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <deque>

#define MAX_MESSAGES 5

AsyncWebServer server(80);
std::deque<String> messages;

// AP credentials
const char* ssid = "ESP32_AP_receiver";
const char* password = "11111111";

void setup() {
  Serial.begin(9600); // For communication with Arduino

  // Set up the ESP32 as an access point
  WiFi.softAP(ssid, password);

  // Print the IP address
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // HTML content with JavaScript to fetch messages
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

  // Serve HTML page
  server.on("/", HTTP_GET, [html](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });

  // Send received messages
  server.on("/messages", HTTP_GET, [](AsyncWebServerRequest *request){
    String messageList;
    for (const String& message : messages) {
      messageList += "Received message: " + message + "\n";
    }
    request->send(200, "text/plain", messageList);
  });

  // Start server
  server.begin();
}

void loop() {
  if (Serial.available()) {
    String receivedMessage = Serial.readStringUntil('\n');
    receivedMessage.trim(); // Remove leading and trailing whitespace

    if (receivedMessage.length() > 0) { // Only add non-empty messages
      Serial.print("Received message: ");
      Serial.println(receivedMessage);

      // Store the received message
      if (messages.size() >= MAX_MESSAGES) {
        messages.pop_front();
      }
      messages.push_back(receivedMessage);
    }
  }
}
