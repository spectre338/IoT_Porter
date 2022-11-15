/*
Deep Sleep Why?
******************************************************
Use deep sleep with touch as a wake up. Store data in
RTC memory to use it over reboots but not resets

This code is under MIT License.

Author:
ACCS
*/

#define Threshold 30 /* Greater the value = higher sensitivity */

RTC_DATA_ATTR int bootCount = 0;
touch_pad_t touchPin;
/*
Print the input that made the ESP32 become awake, but not WOKE
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_why = esp_sleep_get_wakeup_cause();

  switch(wakeup_why)
  {
    case ESP_SLEEP_WAKEUP_TIMER     : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD  : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP       : Serial.println("Wakeup caused by ULP program"); break;
    case ESP_SLEEP_WAKEUP_UNDEFINED : Serial.println("Not sure why I woke up"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
void print_wakeup_touchpad(){
  touch_pad_t pin;

  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin)
  {
    case 0  : Serial.println("Touch detected on PIN 3"); break;
    case 1  : Serial.println("Touch detected on PIN 4"); break;
    case 2  : Serial.println("Touch detected on PIN 5"); break;
    case 3  : Serial.println("Touch detected on PIN 6"); break;
    case 4  : Serial.println("Touch detected on PIN 7"); break;
    case 5  : Serial.println("Touch detected on PIN 8"); break;
    case 6  : Serial.println("Touch detected on PIN 9"); break;
    case 7  : Serial.println("Touch detected on PIN 10"); break;
    default : Serial.println("Wakeup not by touchpad"); break;
  }
}

void callback2(){
  //placeholder 4 callback
}

void callback3(){
  //placeholder 4 callback function
}

void setup(){
  Serial.begin(115200);
  delay(1000); //Takes a second for the Serial Monitor

  //Increment boot number and print it every reboot (Resets on reset)
  ++bootCount;
  Serial.println("Wakes since last reset: " + String(bootCount));

  //Print the why wake reason and which PIN
  print_wakeup_why();
  print_wakeup_touchpad();

  //Setup interrupt on Touch Pad 3 (GPIO15)
  touchAttachInterrupt(T3, callback3, Threshold);
  touchAttachInterrupt(T2, callback2, Threshold);

  //Set TP as wakeup source
  esp_sleep_enable_touchpad_wakeup();

  //Begin Deep Sleep
  Serial.println("Dr. Chandra, will I dream?");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("noreply NEVER PRINTED");
}

void loop(){
  //This will never be exicuted becusea of the method use for deep sleep
}
