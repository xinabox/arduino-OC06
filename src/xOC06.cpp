/*
	This is a library for the OC06 Stepper Driver

	The board uses I2C for communication.
	
	The board communicates with two I2C devices:
	* 	DRV8825  
	*	PCA9554A
	
	Data Sheets:
	DRV8825		- http://www.ti.com/lit/ds/symlink/drv8825.pdf
	PCA9554A	- http://www.ti.com/lit/ds/symlink/pca9554a.pdf
*/

#include "xOC06.h"

/*---Public Function---*/
/********************************************************
 	Constructor
*********************************************************/
xOC06::xOC06(void) 
{
	REG_OUTPUT = 0x00;
	STEP_SIZE = 1;
	PCA9554A_I2C_ADDRESS = 0x38	// I2C Address of Chip
}

xOC06::xOC06(uint8_t addr) 
{
	REG_OUTPUT = 0x00;
	STEP_SIZE = 1;
	PCA9554A_I2C_ADDRESS = addr	// I2C Address of Chip
}
/********************************************************
 	Configure Sensor
*********************************************************/
void xOC06::begin(void)
{
	REG_OUTPUT &= ~MODE0; //Configure for full step
	REG_OUTPUT &= ~MODE1;
	REG_OUTPUT &= ~MODE2;
	
	REG_OUTPUT |= nENABLE; //Disable device by pulling enable high
	
	REG_OUTPUT &= ~DIR; //Sets direction as forward
	
	REG_OUTPUT &= ~DECAY; //Set to slow decay on PWM current
	
	Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
	Wire.write(PCA9554A_REG_OUTPUT);
	Wire.write(REG_OUTPUT); //Sets default output port configuration.
	Wire.endTransmission();
	
	Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
	Wire.write(PCA9554A_REG_CONF);
	Wire.write(0x80); //Configure all pins as output except for P7 which is nFault input.
	Wire.endTransmission();
}

void xOC06::step(void)
{
	REG_OUTPUT |= STEP; //Set step control signal high
	
	Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
	Wire.write(PCA9554A_REG_OUTPUT);
	Wire.write(REG_OUTPUT);
	Wire.endTransmission();
	
	delayMicroseconds(2); // Minimum high time of control pulse
	
	REG_OUTPUT &= ~STEP; //Set step control signal low
	
	Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
	Wire.write(PCA9554A_REG_OUTPUT);
	Wire.write(REG_OUTPUT);
	Wire.endTransmission();
	
	delayMicroseconds(2); // Minimum low time of control pulse
}

void xOC06::setDirection(uint8_t input_dir)
{
	if (input_dir == FORWARD)
	{
		REG_OUTPUT &= ~DIR; //Sets direction as forward
	
		Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
		Wire.write(PCA9554A_REG_OUTPUT);
		Wire.write(REG_OUTPUT);
		Wire.endTransmission();
	}
	else if (input_dir == REVERSE)
	{
		REG_OUTPUT |= DIR; //Sets direction as REVERSE
	
		Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
		Wire.write(PCA9554A_REG_OUTPUT);
		Wire.write(REG_OUTPUT);
		Wire.endTransmission();
	}
}

void xOC06::enable(void)
{
	REG_OUTPUT &= ~nENABLE; //Enable device by pulling enable low
	
	Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
	Wire.write(PCA9554A_REG_OUTPUT);
	Wire.write(REG_OUTPUT);
	Wire.endTransmission();
}

void xOC06::disable(void)
{
	REG_OUTPUT |= nENABLE; //Disable device by pulling enable high
	
	Wire.beginTransmission(PCA9554A_I2C_ADDRESS);
	Wire.write(PCA9554A_REG_OUTPUT);
	Wire.write(REG_OUTPUT);
	Wire.endTransmission();
}

void xOC06::move(long steps, float speed, uint8_t dir)
{
	float delay_time;
	int STEPS_PER_REV;
	
	STEPS_PER_REV = NUM_STEPS*STEP_SIZE;
	
	setDirection(dir);
	
	if (speed < 20)
	{
		delay_time = 60000/((float)STEPS_PER_REV*speed);
		
		for (long i = 0; i < steps; i++)
		{
			step();
			delay((unsigned int)delay_time);
		}
	}
	else
	{
		delay_time = (60000000/((float)STEPS_PER_REV*speed)) - 4;
		
		for (long i = 0; i < steps; i++)
		{
			step();
			delayMicroseconds((unsigned int)delay_time);
		}
	}	
}
