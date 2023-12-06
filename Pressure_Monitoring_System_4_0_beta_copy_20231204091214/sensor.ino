#include "ADS1X15.h"

ADS1115 ADS(0x48);
float f = 0;
double pressure;


void setupADS()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("ADS1X15_LIB_VERSION: ");
  Serial.println(ADS1X15_LIB_VERSION);

  ADS.begin();
  ADS.setGain(1);
  f = ADS.toVoltage();      // voltage factor
  ADS.requestADC(0);     //  first read to trigger
}


int16_t readSensor()
{
  if (ADS.isBusy() == false)
  {
    int16_t val_0 = ADS.getValue();
    ADS.requestADC(0);  // request a new one
    // Serial.print("sensor: ");
    // Serial.print(val_0);
    return(val_0);
  }
}


double calcPressure(int16_t sens){
  double sensVal=sens;
  pressure=(10.0/26400.0)*sens;
  // Serial.print("sensor: ");
  // Serial.print(val_0);
  return (pressure);
}
