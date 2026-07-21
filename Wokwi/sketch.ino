#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>
#include "ecg_data.h"

// ==========================================
// KONFIGURASI WIFI & MQTT
// ==========================================
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// ==========================================
// PIN & SENSOR CONFIG
// ==========================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define GREEN_LED 18
#define RED_LED 19
#define BUZZER 23
#define ONE_WIRE_BUS 4
#define ECG_LENGTH 1000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MPU6050 mpu;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float ecgValue = 0.0;
uint16_t indexData = 0;
bool isNormal = true;
float bodyTemp = 36.5;
bool fallDetected = false;
float accMagnitude = 0;

unsigned long previousECG = 0;
unsigned long previousOLED = 0;
unsigned long previousTemp = 0;
unsigned long previousMQTT = 0;

// ==========================================
// SETUP KONEKSI WIFI & MQTT
// ==========================================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Buat ID client random agar tidakbentrok
    String clientId = "ESP32Client-UGM-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void readMPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accMagnitude = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );

  fallDetected = (accMagnitude > 18);
}

void readTemperature() {
  bodyTemp = sensors.getTempCByIndex(0);
  sensors.requestTemperatures();
}

void updateActuator() {
  if (fallDetected) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, (millis() / 100) % 2);
    if ((millis() / 100) % 2) tone(BUZZER, 2500);
    else noTone(BUZZER);
    return;
  }

  if (isNormal) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    unsigned long cycle = millis() % 1000;
    if (cycle < 300) tone(BUZZER, 1800);
    else noTone(BUZZER);
  }
}

void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  sensors.begin();
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();

  if (!mpu.begin()) {
    Serial.println("MPU6050 Failed");
    while (1);
  }

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  noTone(BUZZER);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (1);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Membaca ECG setiap 10 ms
  if (millis() - previousECG >= 10) {
    previousECG = millis();
    if (isNormal)
      ecgValue = pgm_read_float(&ecg_normal[indexData]);
    else
      ecgValue = pgm_read_float(&ecg_abnormal[indexData]);

    indexData++;
    if (indexData >= ECG_LENGTH) {
      indexData = 0;
      isNormal = !isNormal;
    }
  }

  readMPU();

  if (millis() - previousTemp >= 2000) {
    previousTemp = millis();
    readTemperature();
  }

  updateActuator();

  // Kirim data ke MQTT setiap 500 ms
  if (millis() - previousMQTT >= 500) {
    previousMQTT = millis();

    String timestamp = "21/07/2026 11:55:00";
    float ecgMv = ecgValue * 1000;
    String fallStatus = fallDetected ? "DANGER" : "SAFE";
    String aiStatus = isNormal ? "NORMAL" : "ABNORMAL";

    // Buat payload JSON
    String jsonPayload = "{";
    jsonPayload += "\"timestamp\":\"" + timestamp + "\",";
    jsonPayload += "\"ecg\":" + String(ecgMv, 2) + ",";
    jsonPayload += "\"temperature\":" + String(bodyTemp, 1) + ",";
    jsonPayload += "\"fall\":\"" + fallStatus + "\",";
    jsonPayload += "\"status\":\"" + aiStatus + "\"";
    jsonPayload += "}";

    // Publikasikan ke topik MQTT
    client.publish("ugm/biomedical/telemetry", jsonPayload.c_str());
  }

  // Update OLED setiap 200 ms
  if (millis() - previousOLED >= 200) {
    previousOLED = millis();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("MQTT IOT MONITOR");
    display.println();
    display.print("Status: ");
    display.println(isNormal ? "NORMAL" : "ABNORMAL");
    display.print("ECG   : ");
    display.println(ecgValue * 1000);
    display.print("Temp  : ");
    display.print(bodyTemp, 1);
    display.println(" C");
    display.print("Fall  : ");
    display.println(fallDetected ? "DETECTED" : "SAFE");
    display.display();
  }
}