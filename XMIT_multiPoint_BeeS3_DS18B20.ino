 // Include libraries reqs
#include <WiFi.h>
#include <esp_now.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BeeS3.h>

// Data wire is plugged into port 5 on the Arduino
#define ONE_WIRE_BUS 10

// Setup BeeS3 instance to communicate with Volt Meter
BEES3 bees3;

// String variable for transmit identification (must be unique)
String ident = "30:A0";

// Setup a oneWire instance to communicate with OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass the oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


// Addresses of 5 DS18B20 Temp Probes
uint8_t sensor1[8] = { 0x28, 0x73, 0x53, 0x48, 0xF6, 0x06, 0x3C, 0x13 };
uint8_t sensor2[8] = { 0x28, 0xBF, 0xF7, 0x48, 0xF6, 0x35, 0x3C, 0xA5 };
uint8_t sensor3[8] = { 0x28, 0x5B, 0xE2, 0x48, 0xF6, 0x32, 0x3C, 0x90 };
uint8_t sensor4[8] = { 0x28, 0x45, 0x05, 0x48, 0xF6, 0x37, 0x3C, 0xAC };
uint8_t sensor5[8] = { 0x28, 0x38, 0x3D, 0x48, 0xF6, 0x9D, 0x3C, 0xCD };

// Variables for battery
float batt;
 
// Integer for packet sequence number since reset
int packetCount = 0;
int pac_value;

// Floats for probe temps
float probe1;
float probe2;
float probe3;
float probe4;
float probe5;
 
// Responder MAC Address (Replace with the receiver MAC Address)
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xc2, 0x51, 0x64};
 
// Define data structure
typedef struct struct_message {
  float a;
  float b;
  float c;
  float d;
  float e;
  String f;
  float g;
  int h;
} struct_message;
 
// Create structured data object
struct_message myData;
 
// Register peer in ESP-NOW
esp_now_peer_info_t peerInfo;
 
// Transmit data callback
void OnDataSent(const uint8_t *macAddr, esp_now_send_status_t status)
{
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Transmit Success" : "Transmit Fail");
}
 
void setup() {
 
  // Setup Serial monitor
  Serial.begin(115200);
  delay(100);

    // Initiate BeeS3
  bees3.begin();
 
  // Set ESP32 WiFi mode to Station mode
  WiFi.mode(WIFI_MODE_STA);
 
  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Define callback
  esp_now_register_send_cb(OnDataSent);
 
 
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false; //TODO
 
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
 
}

void batteryLevel() {
  // get battery voltage and store it as "battery"
  float battery = bees3.getBatteryVoltage(); 
  // print battery voltage. 4.2v is full and 3.0v is dead 
  Serial.println(String("Battery: ") + battery);

 
}
// Store the millis of the last battery reading
unsigned long lastBatteryCheck = 0;
// Define the battery check interval as one second
#define BATTERY_CHECK_INTERVAL 10000



void loop() {


  // Read battery 
  batt = bees3.getBatteryVoltage();

  //Iterate packet integer
  pac_value = (packetCount++);

  // Set varables and convert to Farenheit
  sensors.requestTemperatures();
  probe1 = sensors.getTempC(sensor1);
  probe1 = DallasTemperature::toFahrenheit(probe1);
  probe2 = sensors.getTempC(sensor2);
  probe2 = DallasTemperature::toFahrenheit(probe2);
  probe3 = sensors.getTempC(sensor3);
  probe3 = DallasTemperature::toFahrenheit(probe3);
  probe4 = sensors.getTempC(sensor4);
  probe4 = DallasTemperature::toFahrenheit(probe4);
  probe5 = sensors.getTempC(sensor5);
  probe5 = DallasTemperature::toFahrenheit(probe5);
    
    
  // Print values to serial

  Serial.print("Sensor: ");
  Serial.print(ident);
  Serial.print("");
  Serial.print("");
  
  Serial.print("     Probe 1: ");
  printTemperature(sensor1);
  
  Serial.print("     Probe 2: ");
  printTemperature(sensor2);
  
  Serial.print("     Probe 3: ");
  printTemperature(sensor3);
  
  Serial.print("     Probe 4: ");
  printTemperature(sensor4);
  
  Serial.print("     Probe 5: ");
  printTemperature(sensor5);


  
  Serial.print("     Battery: ");
  Serial.println(batt + String("v"));
  
  Serial.print("     Packets sent since last reset: ");
  Serial.println(pac_value);
  Serial.print("");
 
  // Add to structured data object
  myData.a = probe1;
  myData.b = probe2;
  myData.c = probe3;
  myData.d = probe4;
  myData.e = probe5;
  
  myData.f = ident;
  myData.g = batt;
  myData.h = pac_value;
  

  // Send data
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  // wait time between transmits
  delay(5 * 60 * 1000 - 2000);
  
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.println("F");
}
