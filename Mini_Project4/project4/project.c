/*
 * project.c
 *
 *  Created on: Oct 17, 2021
 *      Author: LENOVO
 */


#include "ultrasonic.h"
#include "lcd.h"
#include "avr/io.h"
#include "util/delay.h"

int main(void){
	uint16 distance;
	/*ENABLE I-BIT*/
	SREG |= (1<<7);

	/*initialization of bith drivers*/
	Ultrasonic_init();
	LCD_init();

	LCD_displayString("Distance =   cm");


	while(1){
		/*read the distance*/
		distance = Ultrasonic_readDistance();


		if(distance < 100){
		    LCD_moveCursor(0,10);
			LCD_intgerToString(distance);
			/*to make sure it will remove the rest of the number */
			LCD_displayString(" ");
		}
		else{
			LCD_moveCursor(0,10);
			LCD_intgerToString(distance);

		}


	}


}

