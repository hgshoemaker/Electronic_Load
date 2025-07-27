// Test ADC floating inputs
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1X15 ads;

void setup() {
  Serial.begin(9600);
  Serial.println("ADS1115 Floating Input Test");
  
  ads.begin();
  ads.setGain(GAIN_TWOTHIRDS);  // +/- 6.144V range
  
  Serial.println("Make sure A0 and A1 are completely disconnected");
  Serial.println("Readings should be noisy/random when floating");
  delay(2000);
}

void loop() {
  int16_t adc0, adc1, diff;
  float volts0, volts1, voltsDiff;
  
  // Read individual channels
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  
  // Read differential (A0 - A1)
  diff = ads.readADC_Differential_0_1();
  
  // Convert to voltages
  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);
  voltsDiff = ads.computeVolts(diff);
  
  Serial.print("A0: "); Serial.print(volts0, 4); Serial.print("V  ");
  Serial.print("A1: "); Serial.print(volts1, 4); Serial.print("V  ");
  Serial.print("Diff (A0-A1): "); Serial.print(voltsDiff, 4); Serial.println("V");
  
  delay(500);
}
