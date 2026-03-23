#include <Arduino.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"
#include "secrets.h"

// ============================================
// CONFIGURACIÓN DEL SISTEMA
// ============================================

// Configuración WiFi
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Configuración ThingSpeak
WiFiClient client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Configuración DHT - CORREGIDO
#define DHTPIN 15                    // GPIO4 para el DHT22
#define DHTTYPE DHT22               // DHT22
DHT dht(DHTPIN, DHTTYPE);

// Configuración de tiempos
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;    // Enviar datos cada 30 segundos
const int wifiTimeout = 30;          // Timeout para conexión WiFi

// ============================================
// PROTOTIPOS DE FUNCIONES
// ============================================
void connectToWiFi();
void readAndSendData();
void sendToThingSpeak(float temperature, float humidity);
void printSensorData(float temperature, float humidity);
void printSystemInfo();

// ============================================
// SETUP
// ============================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  printSystemInfo();
  
  dht.begin();
  Serial.println(F("Sensor DHT22 inicializado"));
  delay(500);
  
  connectToWiFi();
  
  ThingSpeak.begin(client);
  Serial.println(F("ThingSpeak inicializado"));
  
  Serial.println(F("\nSistema listo! Enviando datos cada 30 segundos...\n"));
}

// ============================================
// LOOP
// ============================================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi desconectado, reconectando..."));
    connectToWiFi();
  }
  
  if ((millis() - lastTime) > timerDelay) {
    readAndSendData();
    lastTime = millis();
  }
  
  delay(100);
}

// ============================================
// CONECTAR A WiFi
// ============================================
void connectToWiFi() {
  Serial.print(F("\nConectando a WiFi: "));
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < wifiTimeout) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi conectado!"));
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("Error: No se pudo conectar a WiFi"));
  }
}

// ============================================
// LEER SENSOR Y ENVIAR DATOS
// ============================================
void readAndSendData() {
  static int errorCount = 0;
  
  Serial.println(F("\n=== NUEVA LECTURA ==="));
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    errorCount++;
    Serial.print(F("Error #"));
    Serial.print(errorCount);
    Serial.println(F(": No se pudo leer del sensor DHT22"));
    Serial.println(F("  Verifica las conexiones: VCC→3.3V, GND→GND, DATA→GPIO4"));
    
    if (errorCount >= 5) {
      Serial.println(F("Demasiados errores, reiniciando ESP32..."));
      ESP.restart();
    }
    return;
  }
  
  errorCount = 0;
  printSensorData(temperature, humidity);
  sendToThingSpeak(temperature, humidity);
}

// ============================================
// ENVIAR DATOS A THINGSPEAK
// ============================================
void sendToThingSpeak(float temperature, float humidity) {
  Serial.print(F("Enviando a ThingSpeak... "));
  
  // Redondear a 1 decimal
  temperature = round(temperature * 10) / 10.0;
  humidity = round(humidity * 10) / 10.0;
  
  // Configurar campos
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  
  // Enviar
  int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  if (responseCode == 200) {
    Serial.println(F("✅ Éxito!"));
    Serial.print(F("   Datos: "));
    Serial.print(temperature);
    Serial.print(F("°C, "));
    Serial.print(humidity);
    Serial.println(F("%"));
  } else {
    Serial.print(F("❌ Error: Código HTTP "));
    Serial.println(responseCode);
  }
}

// ============================================
// MOSTRAR DATOS DEL SENSOR
// ============================================
void printSensorData(float temperature, float humidity) {
  Serial.println(F("\n LECTURAS DEL SENSOR:"));
  Serial.println(F("  ┌─────────────────────────┐"));
  Serial.print(F("  │   Temperatura: "));
  Serial.print(temperature);
  Serial.println(F(" °C  │"));
  Serial.print(F("  │  Humedad:     "));
  Serial.print(humidity);
  Serial.println(F(" %    │"));
  Serial.println(F("  └─────────────────────────┘"));
}

// ============================================
// MOSTRAR INFORMACIÓN DEL SISTEMA
// ============================================
void printSystemInfo() {
  Serial.println(F("\n╔══════════════════════════════════╗"));
  Serial.println(F("║    SISTEMA DHT22 + THINGSPEAK    ║"));
  Serial.println(F("╠══════════════════════════════════╣"));
  Serial.println(F("║  Versión: 1.0                    ║"));
  Serial.println(F("║  Sensor: DHT22 (GPIO4)           ║"));
  Serial.println(F("║  Intervalo: 30 segundos          ║"));
  Serial.println(F("╚══════════════════════════════════╝\n"));
}