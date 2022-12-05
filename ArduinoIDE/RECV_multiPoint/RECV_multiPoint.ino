// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

// Define data structure
typedef struct struct_message {
    float a;  //temp
    float b;  //humidity
    int c;    //identifier
    float d;  //battery volt
    int e;    //packet seq
} struct_message;
 
// Create structured data object
struct_message myData;
 
// Receive data callback function
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  // Get incoming data
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Print to Serial Monitor
  Serial.println("");
  Serial.print("Sensor "); 
  Serial.print(myData.c);
  Serial.println("");
  
  Serial.print("   Temp: ");
  Serial.println(myData.a);

  Serial.print("   Humidity: ");
  Serial.println(myData.b);
  
  Serial.print("   Voltage: ");
  Serial.println(myData.d); 

  Serial.print("   SEQ#: ");
  Serial.print(myData.e);
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
//  Serial.println(WiFi.localIP());         // Send the IP address 
}
 
void loop() {
  }
