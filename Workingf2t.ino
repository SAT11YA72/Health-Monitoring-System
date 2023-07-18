#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
 
#define REPORTING_PERIOD_MS 1000
 
char auth[] = "HMn8EifyEeDDhNocQ0JaSwsSkWyeHTHF";             // You should get Auth Token in the Blynk App.
char ssid[] = "Nodemuc";
char pass[] = "w8j3ib59";

// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
float GSR=0;
 
 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
    GSR=analogRead(34);
    Blynk.virtualWrite(V6, GSR/10);
}
 
void setup()
{
    Serial.begin(115200);
    
    pinMode(19, OUTPUT);
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
        pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
}
 
void loop()
{
    pox.update();
    Blynk.run();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        //Serial.print("Heart rate:");
        //Serial.print(BPM);
        //Serial.print(" bpm / SpO2:");
        Serial.println(SpO2);
        //Serial.println(" %");
 
        Blynk.virtualWrite(V5, BPM);
        Blynk.virtualWrite(V0, SpO2);
 
        tsLastReport = millis();
    }
}
