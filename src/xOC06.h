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

#ifndef xOC06_h
#define xOC06_h

// Include File Headers
#include "xCore.h"

// Defines PCA9554A Registers
#define PCA9554A_REG_INPUT        	0x00 	// Input register 
#define PCA9554A_REG_OUTPUT       	0x01 	// Output register
#define PCA9554A_REG_POL_INV        0x02 	// Polarity Inversion register
#define PCA9554A_REG_CONF        	0x03 	// Configuration register

//Defines port locations for control signals
#define MODE2		0x01
#define MODE1		0x02
#define MODE0		0x04
#define STEP		0x08
#define nENABLE		0x10
#define DIR			0x20
#define DECAY		0x40
#define nFAULT		0x80

//Define variables
#define FORWARD		0x00
#define REVERSE		0x01
#define NUM_STEPS	200

class xOC06: public xCoreClass
{
	public:
		/**
		* Constructor
		* Creates a new instance of Sensor class.
		*/	
		xOC06();
		xOC06(uint8_t addr);
		/*
		* Runs the setup of the sensor. 
		* Call this in setup(), before reading any sensor data.
		*/		
		void begin(void);
		
		/*
		* Performs a single step in specified DIR. 
		* Call this in loop()
		*/		
		void step(void);
		
		/*
		* Performs a single step in specified DIR. 
		* Call this in loop()
		*/		
		void setDirection(uint8_t input_dir);
		
		/*
		* Enables the stepper driver output 
		* Call this in loop()
		*/		
		void enable(void);
		
		/*
		* Disables the stepper driver output 
		* Call this in loop()
		*/		
		void disable(void);
		
		/*
		* Move number of steps in specified direction at defined speed
		* Call this in loop()
		*/		
		void move(long steps, float speed, uint8_t dir);
		
	private:
		uint8_t PCA9554A_I2C_ADDRESS;
		uint8_t REG_OUTPUT;
		uint8_t STEP_SIZE;
};


#endif