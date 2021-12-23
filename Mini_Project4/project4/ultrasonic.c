/*
 * ultrasonic.c
 *
 *  Created on: Oct 17, 2021
 *      Author: LENOVO
 */


#include "ultrasonic.h"
#include "common_macros.h"
#include "util/delay.h"
/*******************************************************************************
 *                           Private Global Variables                                  *
 *******************************************************************************/

static uint8 g_edgeCount = 0;
static uint16 g_timeHigh = 0;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description
 * This is the call back function called by the ICU driver.
 * This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 * */

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
		/* re-initialize edge counter*/
		g_edgeCount = 0;
	}

}
/* Description
 * Initialize the ICU driver as required.
   Setup the ICU call back function.
   Setup the direction for the trigger pin as output pin through the GPIO driver.
 * */
void Ultrasonic_init(void)
{
	/* initialize the Icu*/
	Icu_ConfigType config = {F_CPU_8,RISING};

	Icu_init(&config);

	/*set the call back function to measure the HIGH period*/
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	/*set TRIGGER pin as output*/
	GPIO_setupPinDirection(TRIGGER_PORT_ID, TRIGGER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_LOW);
}



/* Description
 * Send the Trigger pulse to the ultrasonic
 * */
void Ultrasonic_Trigger(void)
{
    GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_HIGH);
    _delay_us(10);
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_LOW);
}

/* Description
 * Send the trigger pulse by using Ultrasonic_Trigger function.
 * Start the measurements by the ICU from this moment.
 * */
uint16 Ultrasonic_readDistance(void)
{
	uint16 distance;
    /* send the trigger */
	Ultrasonic_Trigger();
    /* wait until the echo signal return*/
	while(g_timeHigh == 0 );
	/* calculate the distance*/
	distance = (uint16)((float)g_timeHigh/58.8);

	if(distance < 60){
		distance++;
	}
	else{
		distance += 2;
		if(distance >= 133){
			distance++;
		}
	}
	/*re-initialize the high time for the next trigger */
	g_timeHigh= 0;

	return distance;
}



