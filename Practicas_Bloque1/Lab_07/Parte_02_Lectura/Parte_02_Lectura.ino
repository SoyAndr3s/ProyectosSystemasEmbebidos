// Lab07_BMP280_OLED.ino 
#include <Wire.h> 
#include <Adafruit_BMP085.h> 
#include <Adafruit_SSD1306.h> 
#include <Adafruit_GFX.h> 
  
#define OLED_W   128 
#define OLED_H    64 
#define OLED_ADDR 0x3C 
  
Adafruit_BMP085 bmp; 
Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1); 
  
// Presion al nivel del mar para calcular altitud (ajustar segun ubicacion) 
const float PRESION_MAR = 1013.25;   // hPa 
  
void setup() { 
    Serial.begin(9600); 
  
    // Iniciar OLED 
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) { 
        Serial.println("ERROR: OLED no encontrada en 0x3C"); 
        while (true); 
    } 
    display.clearDisplay(); 
    display.setTextColor(SSD1306_WHITE); 
    display.setTextSize(1); 
    display.setCursor(20, 28); 
    display.println("Iniciando BMP280..."); 
    display.display(); 
  
    // Iniciar BMP280 
    if (!bmp.begin(0x76)) { 
        Serial.println("ERROR: BMP280 no encontrado en 0x76"); 
        display.clearDisplay(); 
        display.setCursor(5, 28); 
        display.println("ERROR: BMP280"); 
        display.display(); 
        while (true); 
    } 
    Serial.println("BMP280 y OLED listos."); 
    Serial.println("Tiempo\tTemp(C)\tPresion(hPa)\tAltitud(m)"); 
} 
  
void loop() { 
    float temp     = bmp.readTemperature();       // Celsius 
    float presion  = bmp.readPressure() / 100.0; // Pa -> hPa 
    float altitud  = bmp.readAltitude(PRESION_MAR); 
  
    // ── Mostrar en OLED ─────────────────────────────────── 
    display.clearDisplay(); 
  
    // Titulo 
    display.setTextSize(1); 
    display.setCursor(30, 0); 
    display.println("-- BMP280 --"); 
  
    // Temperatura grande 
    display.setTextSize(2); 
    display.setCursor(0, 14); 
    display.print(temp, 1); 
    display.println(" C"); 
  
    // Presion y altitud en texto pequeño 
    display.setTextSize(1); 
    display.setCursor(0, 38); 
    display.print("P: "); 
    display.print(presion, 1); 
    display.println(" hPa"); 
    display.setCursor(0, 50); 
    display.print("Alt: "); 
    display.print(altitud, 0); 
    display.println(" m"); 
  
    display.display();   // Enviar buffer a la pantalla 
  
    // ── Imprimir en Serial ──────────────────────────────── 
    Serial.print(millis()/1000);  Serial.print('\t'); 
    Serial.print(temp, 2);        Serial.print('\t'); 
    Serial.print(presion, 2);     Serial.print('\t'); 
    Serial.println(altitud, 1); 
  
    delay(1000); 
}