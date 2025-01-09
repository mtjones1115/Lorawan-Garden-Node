#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <LittleFS_Mbed_RP2040.h>

// Define variables for Environmental Sensor on SPI Interface
#define SPI_SCK 10
#define SPI_MISO 12
#define SPI_MOSI 11
#define SPI_CS 13

// Define LittleFS settings
#define LFS_MBED_RP2040_VERSION_MIN_TARGET      "LittleFS_Mbed_RP2040 v1.1.0"
#define LFS_MBED_RP2040_VERSION_MIN             1001000

#define _LFS_LOGLEVEL_          1
#define RP2040_FS_SIZE_KB       64

#define FORCE_REFORMAT          false

LittleFS_MBED *myFS;
Adafruit_BME280 bme(SPI_CS, SPI_MOSI, SPI_MISO, SPI_SCK); // software SPI

int sensor1 = 26; 
int dry_val = 686; 
int wet_val = 319; 

void readFile(const char * path)
{
  Serial.print("Reading file: ");
  Serial.print(path);

  FILE *file = fopen(path, "r");

  if (file)
  {
    Serial.println(" => Open OK");
  }
  else
  {
    Serial.println(" => Open Failed");
    return;
  }

  char c;
  uint32_t numRead = 1;

  while (numRead)
  {
    numRead = fread((uint8_t *) &c, sizeof(c), 1, file);

    if (numRead)
      Serial.print(c);
  }

  fclose(file);
}

void setup() 
{
  Serial.begin(115200); 
  pinMode(sensor1, INPUT); 
  while (!Serial)// time to get serial running
    delay(2000); 

  Serial.println(F("BME280 OPERATIONAL"));

  Serial.print("\nStart LittleFS_Test on ");
  Serial.println(BOARD_NAME);
  Serial.println(LFS_MBED_RP2040_VERSION);


#if defined(LFS_MBED_RP2040_VERSION_MIN)

  if (LFS_MBED_RP2040_VERSION_INT < LFS_MBED_RP2040_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(LFS_MBED_RP2040_VERSION_MIN_TARGET);
  }

#endif

  myFS = new LittleFS_MBED();

  if (!myFS->init())
  {
    Serial.println("LITTLEFS Mount Failed");

    return;

  }

  bool status;

  // SPI.begin(); 
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

}

void loop() {

  int moisture1 = analogRead(sensor1);  
  int percentage1 = map(moisture1, dry_val, wet_val, 0, 100); 

  Serial.print("Soil Moisture Percentage: ");
  Serial.print(percentage1);
  Serial.println("%");

  Serial.print("Temperature = ");
  Serial.print((bme.readTemperature()*9/5)+32);
  Serial.println(" *F");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  Serial.print("-----");
  Serial.println("\n");

  delay(1000 * 5); 
}