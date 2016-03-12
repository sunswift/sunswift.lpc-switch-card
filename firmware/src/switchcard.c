/* --------------------------------------------------------------------------                                 
    Switchcard project main
    File name: switchcard
    Author: Charith Perera
    Description: To be completed

    Created: 15/07/2013
   -------------------------------------------------------------------------- */

/* 
 * This file is part of the Sunswift Switchcard project
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

#define DEMO_MODE 0

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


static void channel_0_handler(int value, unsigned int src_time);
static void channel_1_handler(int value, unsigned int src_time);
static void channel_2_handler(int value, unsigned int src_time);
static void channel_3_handler(int value, unsigned int src_time);


void setup(void) {

	GPIO_Init();
	GPIO_SetDir(BASE_RED_LED_PORT, BASE_RED_LED_BIT, 1);
	GPIO_SetDir(BASE_YEL_LED_PORT, BASE_YEL_LED_BIT, 1);
  GPIO_SetDir(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, OUTPUT);
	GPIO_SetDir(SWITCHCARD_CHANNEL_2_PORT, SWITCHCARD_CHANNEL_2_BIT, OUTPUT);
	GPIO_SetDir(SWITCHCARD_CHANNEL_3_PORT, SWITCHCARD_CHANNEL_3_BIT, OUTPUT);
  GPIO_SetDir(SWITCHCARD_CHANNEL_4_PORT, SWITCHCARD_CHANNEL_4_BIT, OUTPUT);

	
  GPIO_SetValue(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, LOW);
	GPIO_SetValue(SWITCHCARD_CHANNEL_2_PORT, SWITCHCARD_CHANNEL_2_BIT, LOW);
	GPIO_SetValue(SWITCHCARD_CHANNEL_3_PORT, SWITCHCARD_CHANNEL_3_BIT, LOW);
	GPIO_SetValue(SWITCHCARD_CHANNEL_4_PORT, SWITCHCARD_CHANNEL_4_BIT, LOW);
	
	
	/* Initialise UART0 */
	UART_Init(115200);
	
	/* Initialise Scandal, registers for the config messages, timesync messages and in channels */
	scandal_init();
		
	/* Registering in-channel handlers */
	scandal_register_in_channel_handler(0, (in_channel_handler) channel_0_handler);
	scandal_register_in_channel_handler(1, (in_channel_handler) channel_1_handler);
	scandal_register_in_channel_handler(2, (in_channel_handler) channel_2_handler);
	scandal_register_in_channel_handler(3, (in_channel_handler) channel_3_handler);
	
	
	/* Display welcome header over UART */
	UART_printf("Switchcard - setup completed and reporting for duty\n\r");
	
} 



int main(void) {

#if DEMO_MODE
  int32_t blinkStat=1;
  sc_time_t one_sec_timer = sc_get_timer();
#endif
	setup();
	
	red_led(0);

	UART_printf("entering loop\r\n");
    
  while(1){
  
    handle_scandal();
  
#if DEMO_MODE
    if ((sc_get_timer() + 1000) > one_sec_timer ){
      one_sec_timer = one_sec_timer + 800;
      UART_printf("blink %d\r\n", blinkStat);
      channel_0_handler(blinkStat, one_sec_timer);
      channel_1_handler(blinkStat, one_sec_timer);
      channel_2_handler(1, one_sec_timer);
      blinkStat = !blinkStat;
    }
#endif
  }
}

static void channel_0_handler(int value, unsigned int src_time){
	UART_printf("Received a CAN message to turn on Channel 0, Value=%d, Time=%u\n\r", value, src_time);
	if(value){
		GPIO_SetValue(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, HIGH);
	}else{
		GPIO_SetValue(SWITCHCARD_CHANNEL_1_PORT, SWITCHCARD_CHANNEL_1_BIT, LOW);
	}
}

static void channel_1_handler(int value, unsigned int src_time){
	UART_printf("Received a CAN message to turn on Channel 1, Value=%d, Time=%u\n\r", value, src_time);
	if(value){
		GPIO_SetValue(SWITCHCARD_CHANNEL_2_PORT, SWITCHCARD_CHANNEL_2_BIT, HIGH);
	}else{
		GPIO_SetValue(SWITCHCARD_CHANNEL_2_PORT, SWITCHCARD_CHANNEL_2_BIT, LOW);
	}
}

static void channel_2_handler(int value, unsigned int src_time){
	UART_printf("Received a CAN message to turn on Channel 2, Value=%d, Time=%u\n\r", value, src_time);
	if(value){
		GPIO_SetValue(SWITCHCARD_CHANNEL_3_PORT, SWITCHCARD_CHANNEL_3_BIT, HIGH);
	}else{
		GPIO_SetValue(SWITCHCARD_CHANNEL_3_PORT, SWITCHCARD_CHANNEL_3_BIT, LOW);
	}
}

static void channel_3_handler(int value, unsigned int src_time){
	UART_printf("Received a CAN message to turn on Channel 3, Value=%d, Time=%u\n\r", value, src_time);
	if(value){
		GPIO_SetValue(SWITCHCARD_CHANNEL_4_PORT, SWITCHCARD_CHANNEL_4_BIT, HIGH);
	}else{
		GPIO_SetValue(SWITCHCARD_CHANNEL_4_PORT, SWITCHCARD_CHANNEL_4_BIT, LOW);
	}
}