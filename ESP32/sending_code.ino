#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "srujan";
const char* password = "11111111";

AsyncWebServer server(80);

void setup() {
  Serial.begin(9600); // For communication with Arduino
  Serial.println("ESP32 is ready");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print local IP address
  Serial.println(WiFi.localIP());

  // HTML content with JavaScript for popup and reload
  const char* html = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head><title>LoRa Sender</title></head>
  <script>
    function sendMessage(event) {
      event.preventDefault();
      var xhr = new XMLHttpRequest();
      var url = "/send?message=" + encodeURIComponent(document.getElementById("message").value);
      xhr.open("GET", url, true);
      xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
          alert(xhr.responseText);
          window.location.reload();
        }
      };
      xhr.send();
    }
  </script>
  </head>
  <body>
    <h1>Enter text to send via LoRa</h1>
    <form onsubmit="sendMessage(event)">
      <input type="text" id="message" name="message">
      <input type="submit" value="Send">
    </form>
  </body>
  </html>)rawliteral";

  // Serve HTML page
  server.on("/", HTTP_GET, [html](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });

  // Send message
  server.on("/send", HTTP_GET, [](AsyncWebServerRequest *request){
    String message;
    if (request->hasParam("message")) {
      message = request->getParam("message")->value();
      Serial.println(message);
      request->send(200, "text/plain", "Message sent: " + message);
    } else {
      request->send(200, "text/plain", "No message sent");
    }
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing needed here
}
