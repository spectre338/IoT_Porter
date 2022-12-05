// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

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
 
// Callback function
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  // Get incoming data
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Print to Serial Monitor
  Serial.println("");
  Serial.print("Sensor "); 
  Serial.print(myData.f);
  Serial.println("");
  
  Serial.print("   Probe1: ");
  Serial.println(myData.a);

  Serial.print("   Probe2: ");
  Serial.println(myData.b);
  
  Serial.print("   Probe3: ");
  Serial.println(myData.c);
  
  Serial.print("   Probe4: ");
  Serial.println(myData.d);
  
  Serial.print("   Probe5: ");
  Serial.println(myData.e);
  
  Serial.print("   Voltage: ");
  Serial.println(myData.g); 

  Serial.print("   SEQ#: ");
  Serial.print(myData.h);
  Serial.println("");
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
 
  // Start ESP32 in Station mode
  WiFi.mode(WIFI_MODE_STA);
 
  // Initalize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);

  //  WiFi.begin(ssid, password);             // Connect to the network
//  Serial.print("Connecting to ");
//  Serial.print(ssid);

//  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
//   delay(500);
//   Serial.print('.');
// }

//  Serial.println('\n');
//  Serial.println("Connection established!");  
//  Serial.print("IP address:\t");
//  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
 
void loop() {
  }
