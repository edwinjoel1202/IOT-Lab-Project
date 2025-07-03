#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi credentials
const char* ssid = "Thor's Apple";
const char* password = "thormjolnir";

// Servo setup
Servo servo;
int servoPin = 2;  // D4 on NodeMCU (GPIO 2)

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Confirmed address 0x27

// Web server on port 80
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);  // Wait for Serial Monitor

  // Initialize LCD
  Wire.begin(4, 5);  // SDA = GPIO 4 (D2), SCL = GPIO 5 (D1)
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pet Feeder");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  // Servo setup
  servo.attach(servoPin);
  servo.write(0);  // Initial position (closed)

  // Web server routes
  server.on("/", handleRoot);      // Home page
  server.on("/feed", handleFeed);  // Feed endpoint
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();  // Handle incoming requests
}

void handleRoot() {
  server.send(200, "text/plain", "Pet Feeder Ready");
}

void handleFeed() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Feeding...");
  Serial.println("Feeding started");
  servo.write(180);
  delay(3000);
  servo.write(0);
  Serial.println("Feeding complete");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Feed Complete");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pet Feeder");
  lcd.setCursor(0, 1);
  lcd.print("Idle");
  server.sendHeader("Access-Control-Allow-Origin", "*");  // Add CORS header
  server.send(200, "text/plain", "Feed dispensed");
}