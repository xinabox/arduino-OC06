/*************************************************************
  This is an examples for the OC06
  Stepper Driver

	You can buy one on our store!
	-----> https://xinabox.cc/products/OC06/

  This examples Steps the motor at 10 RPM

  The driver over the I2C Bus.

*************************************************************/

#include <xCore.h>
#include "xOC06.h"

xOC06 OC06;

void setup() {
  Wire.begin();
  
  OC06.begin();
  OC06.enable();
}
void loop() {
  
  OC06.move(5000, 200, FORWARD);
  delay(500);
  OC06.move(5000, 200, REVERSE);
  delay(500);
}
