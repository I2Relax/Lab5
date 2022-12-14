/*
 * fsm.h
 *
 *  Created on: Dec 13, 2022
 *      Author: Acer
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"

extern uint8_t command_flag;
extern uint8_t command_data[30];
extern int status;
extern int com_status;

#define INIT			  0
#define GET_COM			  1
#define REC_COM  		  2
#define SEND_ADC		  3


void command_parser_fsm() ;
void uart_communication_fsm();

#endif /* INC_FSM_H_ */
