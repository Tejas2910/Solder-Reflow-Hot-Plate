/*
 * myLib.h
 *
 *  Created on: 21 Mar 2023
 *      Author: TEJAS AMRITKAR
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#include "inc/tm4c123gh6pm.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"
#include "driverlib/pin_map.h"
#include"driverlib/debug.h"
#include"driverlib/adc.h"


#define PORT_E GPIO_PORTE_BASE
#define ADC_0 ADC0_BASE

void adc_init(void);
int ADC_conv(void);
void delay_ms(float del);
void relay_init();
void relay_on(int number);
float volt_to_resistor(int ptvolt);
int resistor_to_temp(float resistor);
#endif /* MYLIB_H_ */
