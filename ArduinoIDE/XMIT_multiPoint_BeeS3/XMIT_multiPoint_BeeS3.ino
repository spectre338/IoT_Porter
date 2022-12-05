 // Include libraries reqs
#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>
#include <BeeS3.h>

BEES3 bees3;

// Define DHT11 
#define DHTPin 10
#define DHTType DHT11
 
// Create DHT Object
DHT dht(DHTPin, DHTType);
 
// Variables for temperature and humidity
float temp;
float humid;

// Variables for battery
float batt;
 
// Integer variable for transmit identification (must be unique)
int ident = 4;

// Integer for packet sequence number since reset
int packetCount = 0;
int pac_value;

 
// Responder MAC Address (Replace with the receiver MAC Address)
uint8_t broadcastAddress[] = {0x0C, 0xB8, 0x15, 0xc2, 0x51, 0x64};
 
// Define data structure
typedef struct struct_message {
  float a;
  float b;
  int c;
  float d;
  int e;
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
 
  // Initiate DHT11
  dht.begin();
 
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
  peerInfo.encrypt = false;
 
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
 
  // Read DHT11 module values
  temp = dht.readTemperature(true);  // returns F
  delay(1000);
  humid = dht.readHumidity();
  delay(1000);

  // Read battery 
  batt = bees3.getBatteryVoltage();

  //Iterate packet integer
  pac_value = (packetCount++);

  // Print values to serial

  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Humid: ");
  Serial.println(humid);
  Serial.print("Battery: ");
  Serial.println(batt + String("v"));
  Serial.print("Packets sent since last boot: ");
  Serial.println(pac_value);
 
  // Add to structured data object
  myData.a = temp;
  myData.b = humid;
  myData.c = ident;
  myData.d = batt;
  myData.e = pac_value;
  

  // Send data
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  // wait time between transmits
  delay(1 * 60 * 1000 - 2000);
}
