/* 
MANEJO DE ERRORES DEL SISTEMA
 

En este programa se utilizan variables booleanas para detectar
fallos en los diferentes componentes de la estación meteorológica.

errorDHT  -> Se activa cuando el sensor DHT no responde o devuelve valores inválidos (NaN).

errorSD   -> Se activa cuando hay un problema con la tarjeta SD, por ejemplo: 
            no se inicializa correctamente o falla la escritura del archivo.

 errorBMP  -> Se activa cuando el sensor BMP280 no responde o no se encuentra.

 errorSensor -> Es una variable general que indica si existe algún error en el sistema.

 Esta variable se construye así:
 errorSensor = errorSD || errorDHT;

Si cualquiera de los sensores falla, errorSensor será TRUE, o si todos funcionan correctamente, será FALSE.

en caso de TRUE la condicional "if (errorSensor) digitalWrite(PIN_LED_ROJO, HIGH);"
indicara el fallo encendiendo el led rojo
*/

#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>
#include <SPI.h>

// ---------- Pines ESP32 ----------
#define PIN_DHT       4
#define PIN_LDR       34
#define PIN_PULSADOR  15
#define PIN_LED_VERDE 25
#define PIN_LED_AZUL  26
#define PIN_LED_ROJO  27
#define PIN_SD_CS     5

// ---------- Sensores ----------
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);
Adafruit_BMP280 bmp;

// ---------- OLED ----------
#define OLED_ANCHO 128
#define OLED_ALTO  64
#define OLED_RESET -1
Adafruit_SSD1306 oled(OLED_ANCHO, OLED_ALTO, &Wire, OLED_RESET);

// ---------- Tiempos ----------
unsigned long tAnteriorSensores = 0;
unsigned long tAnteriorOLED     = 0;
unsigned long tAnteriorLDR      = 0;

const long INTERVALO_SENSORES = 5000;
const long INTERVALO_LDR      = 1000;
const long INTERVALO_OLED     = 3000;

// ---------- Estado ----------
bool grabandoSD = false;
bool errorSensor = false;
byte pantallaActual = 0;
bool errorSD = false;
bool errorDHT = false;

// ---------- Datos ----------
float tempDHT = 0, humDHT = 0;
float tempBMP = 0, presionBMP = 0, altBMP = 0;
int valorLDR = 0;
float luzPct = 0;

// ---------- Botón ----------
int estadoBoton = HIGH;
int estadoBotonAnterior = HIGH;
unsigned long tRebote = 0;
const long DEBOUNCE = 50;

// ---------- Archivo ----------
const char NOMBRE_CSV[] = "/data.csv";

// ============================================================
void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AZUL, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  // I2C (OLED + BMP)
  Wire.begin(21, 22);

  // OLED
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error OLED");
    errorSensor = true;
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  // Sensores
  dht.begin();

  if (!bmp.begin(0x76)) {
    Serial.println("Error BMP280");
    errorSensor = true;
  }

  // SD
  // ---------- SD ----------
  if (!SD.begin(PIN_SD_CS)) {
    Serial.println("Error SD");
    errorSD = true;
  } else {
    errorSD = false;

    // Crear archivo con encabezado si no existe
    if (!SD.exists(NOMBRE_CSV)) {
      File archivo = SD.open(NOMBRE_CSV, FILE_WRITE);
      if (archivo) {
        archivo.println("tiempo,temp_dht,hum,temp_bmp,presion,luz");
        archivo.close();
        Serial.println("Encabezado creado");
      } else {
        Serial.println("Error creando archivo");
        errorSD = true;
      }
    }
  }

  // Manejo global de errores
  errorSensor = errorSD || errorDHT;

  actualizarLED();
}

// ============================================================
void loop() {
  unsigned long ahora = millis();

  // BOTÓN
  int lectura = digitalRead(PIN_PULSADOR);

  if (lectura != estadoBotonAnterior) {
    tRebote = ahora;
  }

  if ((ahora - tRebote) > DEBOUNCE) {
    if (lectura != estadoBoton) {
      estadoBoton = lectura;
      if (estadoBoton == LOW) {
        grabandoSD = !grabandoSD;
        actualizarLED();
      }
    }
  }

  estadoBotonAnterior = lectura;

  // SENSORES
  if (ahora - tAnteriorSensores >= INTERVALO_SENSORES) {
    tAnteriorSensores = ahora;
    leerSensores();

    enviarSerial();

    if (grabandoSD) {
      escribirCSV();
    }
  }

  // LDR
  if (ahora - tAnteriorLDR >= INTERVALO_LDR) {
    tAnteriorLDR = ahora;
    valorLDR = analogRead(PIN_LDR);
    luzPct = (valorLDR / 4095.0) * 100.0; // ESP32 es 12 bits
  }

  // OLED
  if (ahora - tAnteriorOLED >= INTERVALO_OLED) {
    tAnteriorOLED = ahora;
    pantallaActual = (pantallaActual + 1) % 3;
    mostrarOLED();
  }
}

// ============================================================
void leerSensores() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    tempDHT = t;
    humDHT = h;
  }
  if (isnan(t) || isnan(h)) {
    errorSensor = true;
  } else {
    tempDHT = t;
    humDHT = h;
    errorDHT = false;
  }
  errorSensor = errorSD || errorDHT;

  tempBMP = bmp.readTemperature();
  presionBMP = bmp.readPressure() / 100.0F;
  altBMP = bmp.readAltitude(1013.25);

  actualizarLED();
}

// ============================================================
void escribirCSV() {
  File archivo = SD.open(NOMBRE_CSV, FILE_APPEND);

  if (archivo) {
    archivo.print(millis()); archivo.print(",");
    archivo.print(tempDHT); archivo.print(",");
    archivo.print(humDHT); archivo.print(",");
    archivo.print(tempBMP); archivo.print(",");
    archivo.print(presionBMP); archivo.print(",");
    archivo.println((int)luzPct);
    archivo.close();
  }
}

// ============================================================
void mostrarOLED() {
  oled.clearDisplay();
  oled.setCursor(0, 0);

  if (pantallaActual == 0) {
    oled.println("TEMP + HUMEDAD");
    oled.print("DHT: "); oled.println(tempDHT);
    oled.print("HUM: "); oled.println(humDHT);
    oled.print("BMP: "); oled.println(tempBMP);

  } else if (pantallaActual == 1) {
    oled.println("PRESION + ALT");
    oled.print("Pres: "); oled.println(presionBMP);
    oled.print("Alt: "); oled.println(altBMP);

  } else {
    oled.println("LUZ + ESTADO");
    oled.print("Luz: "); oled.println((int)luzPct);
    oled.println(grabandoSD ? "GRABANDO" : "PAUSADO");
  }

  oled.display();
}

// ============================================================
void actualizarLED() {
  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_AZUL, LOW);
  digitalWrite(PIN_LED_ROJO, LOW);

  if (errorSensor) digitalWrite(PIN_LED_ROJO, HIGH);
  else if (grabandoSD) digitalWrite(PIN_LED_AZUL, HIGH);
  else digitalWrite(PIN_LED_VERDE, HIGH);
}

void enviarSerial() {
  Serial.print("Temp DHT: "); Serial.print(tempDHT);
  Serial.print(" | Hum: "); Serial.print(humDHT);
  Serial.print(" | Temp BMP: "); Serial.print(tempBMP);
  Serial.print(" | Presion: "); Serial.print(presionBMP);
  Serial.print(" | Luz: "); Serial.print((int)luzPct);
  Serial.print(" | Estado: ");
  Serial.println(grabandoSD ? "GRABANDO" : "PAUSADO");
}