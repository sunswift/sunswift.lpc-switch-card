/* --------------------------------------------------------------------------                                 
    Template project main
    File name: template.c
    Author: Etienne Le Sueur
    Description: The template main file. It provides a simple example of using
                 some standard scandal functions such as the UART library, the
                 CAN library, timers, LEDs, GPIOs.
                 It is designed to compile and work for the 3 micros we use on
                 the car currently, the MSP430F149 and MCP2515 combination and
                 the LPC11C14 and LPC1768 which have built in CAN controllers.

                 UART_printf is provided by the Scandal stdio library and 
                 should work well on all 3 micros.

                 If you are using this as the base for a new project, you
                 should first change the name of this file to the name of
                 your project, and then in the top level makefile, you should
                 change the CHIP and MAIN_NAME variables to correspond to
                 your hardware.

                 Don't be scared of Scandal. Dig into the code and try to
                 understand what's going on. If you think of an improvement
                 to any of the functions, then go ahead and implement it.
                 However, before committing the changes to the Scandal repo,
                 you should discuss with someone else to ensure that what 
                 you've done is a good thing ;-)

                 Keep in mind that this code is live to the public on
                 Google Code. No profanity in comments please!

    Copyright (C) Etienne Le Sueur, 2011

    Created: 07/09/2011
   -------------------------------------------------------------------------- */

/* 
 * This file is part of the Sunswift Template project
 * 
 * This template is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with the project.  If not, see <http://www.gnu.org/licenses/>.
 */

#undef 	DOUBLE_BUFFER_EXAMPLE
#undef 	IN_CHANNEL_EXAMPLE
#undef WAVESCULPTOR_EXAMPLE

#define BASE_YEL_LED_PORT 	3
#define BASE_YEL_LED_BIT 	1
#define BASE_RED_LED_PORT 	3
#define BASE_RED_LED_BIT 	0

#define SWITCHCARD_CHANNEL_1_PORT	0
#define SWITCHCARD_CHANNEL_1_BIT	9
#define SWITCHCARD_CHANNEL_2_PORT	0
#define SWITCHCARD_CHANNEL_2_BIT	8
#define SWITCHCARD_CHANNEL_3_PORT	1
#define SWITCHCARD_CHANNEL_3_BIT	1
#define SWITCHCARD_CHANNEL_4_PORT	1
#define SWITCHCARD_CHANNEL_4_BIT	2



#include <scandal/engine.h>
#include <scandal/message.h>
#include <scandal/leds.h>
#include <scandal/utils.h>
#include <scandal/uart.h>
#include <scandal/stdio.h>
#include <scandal/wdt.h>
#include <scandal/wavesculptor.h>

#include <string.h>


#include <project/driver_config.h>
#include <project/target_config.h>
#include <arch/can.h>
#include <arch/uart.h>
#include <arch/timer.h>
#include <arch/gpio.h>
#include <arch/types.h>
#include <arch/i2c.h>
#include <arch/ssp.h>





void setup(void) {

	/* Initialise the watchdog timer. If the node crashes, it will restart automatically */
	//WDT_Init();  //TODO!
	
	GPIO_Init();
	GPIO_SetDir(BASE_RED_LED_PORT, BASE_RED_LED_BIT, 1);
	GPIO_SetDir(BASE_YEL_LED_PORT, BASE_YEL_LED_BIT, 1);
    GPIO_SetDir(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, 1);
	GPIO_SetDir(SWITCHCARD_CHANNEL_2_PORT, SWITCHCARD_CHANNEL_2_BIT, 1);
	GPIO_SetDir(SWITCHCARD_CHANNEL_3_PORT, SWITCHCARD_CHANNEL_3_BIT, 1);
    GPIO_SetDir(SWITCHCARD_CHANNEL_4_PORT, SWITCHCARD_CHANNEL_4_BIT, 1);

	
    GPIO_SetValue(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, 0);
	/* Initialise Scandal, registers for the config messages, timesync messages and in channels */
	scandal_init();
	
	/* Initialise UART0 */
	UART_Init(115200);
	
	/* Display welcome header over UART */
	UART_printf("Switchcard - setup completed and reporting for duty\n\r");
	
} // setup



/* This is your main function! You should have an infinite loop in here that
 * does all the important stuff your node was designed for */
int main(void) {
	setup();

	/* Wait until UART is ready */
	//scandal_delay(100);

	

	red_led(1);

	sc_time_t one_sec_timer = sc_get_timer();


    while(1){
        GPIO_SetValue(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, 1);
		scandal_delay(500);
		GPIO_SetValue(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, 0);
		scandal_delay(3500);
    }



}


