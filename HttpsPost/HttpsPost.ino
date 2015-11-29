#include <Wire.h>
#include "rgb_lcd.h"
#include "IotHttpClient.h"
#include "IotUtils.h"

#include <LWiFi.h>
#include <string>

// ********************************************
// ****** START user defined definitions ******
// ********************************************
#define WIFI_SSID                       "Geraldinephone"
#define WIFI_PASSWORD                   "123456789"
// ******************************************
// ****** END user defined definitions ******
// ******************************************

#define pinTouch 3

rgb_lcd lcd;
const int colorR = 255;
const int colorG = 50;
const int colorB = 50;

// ******************************************
// ****** setup()****************************
// ******************************************
void setup()
{
  
    Serial.begin(9600);
    pinMode(pinTouch, INPUT);
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    LWiFi.begin();
    delay(6000);
    
}

// ******************************************
// ****** loop() ****************************
// ******************************************
void loop()
{
  // Read the input on analog pin 0:
  float sensorValue = analogRead(A0);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // Print out the value you read:
  Serial.println(voltage);
  
  // Wait 100 milliseconds
  delay(1000);
  
  int state = digitalRead(pinTouch);
  if (state == 1) {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Sending for");
    Serial.println("Sending for");
    lcd.setCursor(0,1);
    lcd.print("housekeeping");
    Serial.println("housekeeping");  
    delay(100);
  
    
  }
  
    else {
    lcd.setCursor(1,1);
    lcd.print("Touch for");
    Serial.println("Touch for");
    lcd.setCursor(0,1);
    lcd.print("housekeeping");
    Serial.println("housekeeping");  
  }
  
    Serial.print("\nSearching for Wifi AP...\n");

    if ( LWiFi.connectWPA(WIFI_SSID, WIFI_PASSWORD) != 1 )
    {
        Serial.println("Failed to Connect to Wifi.");
    }
    else
    {
        Serial.println("Connected to WiFi");
        
     //   lcd.setCursor(0, 0);
       // lcd.print("Connected WiFi");
        
        // Generate some random data to send to Azure cloud.
        srand(vm_ust_get_current_time());

        int device_id = 1 + (rand() % 50);
        int pressure = (voltage);

        // Construct a JSON data string using the random data.
        std::string json_iot_data;
        
        json_iot_data += "{ \"DeviceId\":" + IotUtils::IntToString(device_id);
        json_iot_data += ", \"PressureData\":" + IotUtils::IntToString(voltage);
        json_iot_data += " }";

        // Send the JSON data to the Azure cloud and get the HTTP status code.
        IotHttpClient     https_client;
        
        int http_code = https_client.SendAzureHttpsData(json_iot_data);

        Serial.println("Print HTTP Code:" + String(http_code));
        //lcd.setCursor(0, 1);
        //lcd.print("Code:" + String(http_code));
    }
  //  Serial.println("Disconneting HTTP connection");
    LWiFi.disconnect();
    
    // Sleeps for a while...
    delay(3000);
}




