#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Ganor1";
const char* password = "4085429360";
const char* host = "api.openweathermap.org";
const char* apiKey = "4057afe9c3e305a34bb9fdd2bb3efe92";
const char* location = "Tel%20Aviv,il";

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  Serial.print("Getting forecast temperature for Tel Aviv...");
  getForecastTemperature();
}

void loop() {
  // Do nothing
}

void getForecastTemperature() {
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("Connection failed.");
    return;
  }

  // Create the HTTP request URL
  String url = "/data/2.5/weather?q=" + String(location) + "&appid=" + String(apiKey);

  // Send the HTTP request
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Wait for the server's response
  while (!client.available()) {
    delay(100);
  }

  // Read and parse the JSON response
  DynamicJsonDocument jsonDoc(1024);
  DeserializationError error = deserializeJson(jsonDoc, client);
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Extract the forecast temperature from the JSON response
  JsonObject main = jsonDoc["main"];
  float temperature = main["temp"];
  temperature -= 273.15;  // Convert temperature from Kelvin to Celsius

  Serial.print("Forecast temperature for Tel Aviv: ");
  Serial.print(temperature);
  Serial.println(" °C");
}
