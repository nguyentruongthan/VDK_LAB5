/*
 * fsm_uart_communiation.c
 *
 *  Created on: Dec 18, 2023
 *      Author: than
 */

#include "fsm_uart_communiation.h"

#define INIT		0
#define READ_ADC	1
#define SHOW_ADC	2

uint8_t state_uart_communiation_fsm = INIT;
uint32_t adc_value = 0;
void uart_communiation_fsm(){
	switch(state_uart_communiation_fsm){
	case INIT:
		state_uart_communiation_fsm = READ_ADC;
		break;
	case READ_ADC:
		if(read_adc_flag){//if flag indicate read adc is set
			read_adc_flag = 0;
			//TODO
			//read adc
			adc_value = HAL_ADC_GetValue(&hadc1);

			//print adc value through uart
			uint8_t adc_str[20];
			uint16_t size = sprintf(adc_str, "!%d#\r\n", adc_value);
			HAL_UART_Transmit(&huart2, adc_str, size, 1000);

			//set timer for print value of adc every 3 second unitl receive OK from uart
			set_timeout_ok(300);//3 seconds

			state_uart_communiation_fsm = SHOW_ADC;
		}
		break;
	case SHOW_ADC:
		if(ok_flag){
			ok_flag = 0;
			state_uart_communiation_fsm = READ_ADC;
			return;
		}

		if(read_adc_flag){
			state_uart_communiation_fsm = READ_ADC;
			return;
		}
		if(get_timeout_ok()){
			set_timeout_ok(300);//3 seconds

			uint8_t adc_str[20];
			uint16_t size = sprintf(adc_str, "!%d#\r\n", adc_value);
			HAL_UART_Transmit(&huart2, adc_str, size, 1000);
		}
		break;
	default: break;
	}
}
