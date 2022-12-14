/*
 * fsm.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Acer
 */

#include "software_timer.h"
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "fsm.h"

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

uint8_t command_flag;
uint8_t command_data[30];

int status=INIT;
int com_status=REC_COM;

void strcopy(uint8_t* str1, uint8_t* str2) {
	for (int i=0;i<MAX_BUFFER_SIZE;i++) {
		str1[i]=str2[i];
	}
}

uint8_t compare(uint8_t* str1, uint8_t* str2) {
	for (int i=0;i<MAX_BUFFER_SIZE;i++) {
		if (str1[i]!=str2[i]) return 0;
		if (str1[i]=='#') return 1;
	}
	return 1;
}

void command_parser_fsm(){
	switch (status) {
		case INIT:
			if (buffer[index_buffer-1]=='!') {
				status=GET_COM;
			}
			else {
				status=INIT;
				index_buffer=0;
			}
			break;
		case GET_COM:
			if (index_buffer>29) {
				status=INIT;
				index_buffer=0;
			}
			if (buffer[index_buffer-1]=='#') {
				strcopy(command_data, buffer);
				command_flag=1;
				status=INIT;
				index_buffer=0;
			}
			break;
		default:
			break;
	}
}

void uart_communication_fsm(){
	char str[50];
	switch (com_status) {
		case REC_COM:
			ADC_value=HAL_ADC_GetValue(&hadc1);
			//check if command if "!RST#" or not
			if (command_flag==1) {
				uint8_t temp[MAX_BUFFER_SIZE]="!RST#";
				if (compare(command_data, temp)==1) {
					com_status=SEND_ADC;
					setTimer1(1);
				}
				command_flag=0;
			}
			break;
		case SEND_ADC:
			//check if command is "!OK#" or not
			if (command_flag==1) {
				uint8_t temp[MAX_BUFFER_SIZE]="!OK#";
				if (compare(command_data, temp)==1) {
					setTimer1(0);
					com_status=REC_COM;
				}
				command_flag=0;
			}
			if (timer1_flag==1) {
				//print adc
				HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "!ADC=%d#\r\n", ADC_value), 1000);
				//set timer 3s
				setTimer1(3000);
			}
			break;
		default:
			break;
	}
}
