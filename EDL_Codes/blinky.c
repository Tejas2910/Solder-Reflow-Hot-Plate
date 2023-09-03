
#include "ST7735.h"
#include "myLib.h"
int temp_seconds[300];
int ptvolt;
float error;
//#include "temp_sensor.h"
void delay_ms(float del)     //generates delay in milliseconds
{
float del1 = SysCtlClockGet()/1000;
SysCtlDelay(del1*del);
}

int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
    LcdInit(); // 128x160 pixels
    fillScreen(ST7735_BLACK);
    delay_ms(40);
    fillRoundRect(20, 10, 88, 60, 8, ST7735_WHITE);
    delay_ms(40);
    drawString("ptvolt", 25, 20, ST7735_BLACK, ST7735_WHITE, 1);
    delay_ms(40);
    fillRoundRect(20, 80, 88, 60, 8, ST7735_WHITE);
    delay_ms(40);
    drawString("Time", 25, 90, ST7735_BLACK, ST7735_WHITE, 1);
    delay_ms(40);
    drawString("status", 45, 35, ST7735_BLACK, ST7735_WHITE, 1);
    delay_ms(40);
    drawString("temp", 25, 110, ST7735_BLACK, ST7735_WHITE, 1);
    delay_ms(40);
    drawString("resistor", 25, 130, ST7735_BLACK, ST7735_WHITE, 1);
    delay_ms(40);
    int abcd =  SysCtlClockGet();
    adc_init();                         // init hardware via Xware
    relay_init();
    int ptvolt_high, ptvolt_low;
    float volt, last_error, Kp, Kd, Ki, controller;
    int time = 0;
    ptvolt_high = 3500;
    ptvolt_low = 1900;
    int temp=0;
    error = 0;
    last_error = 0;
    Kp = 1;
    Kd = 2;
    Ki = 0.001;
    time = 0;
    controller = 0;
    float resistor;
    while (1){
        ptvolt = ADC_conv();    // gives the value of temperature
        resistor = volt_to_resistor(ptvolt);
        temp = resistor_to_temp(resistor);
        volt = (ptvolt-ptvolt_low)/(ptvolt_high-ptvolt_low);
        error = 1 - volt;
        controller = Kp*error+Kd*(volt-last_error)+Ki*(error+last_error);
        last_error = error + last_error;
        delay_ms(40);                             // create a delay of ~1/2sec
        drawNumber(ptvolt, 70, 20, ST7735_BLACK, ST7735_WHITE, 1);
        delay_ms(40);
        drawNumber(time, 70, 90, ST7735_BLACK, ST7735_WHITE, 1);
        delay_ms(40);
        drawNumber(temp, 70, 110, ST7735_BLACK, ST7735_WHITE, 1);
        delay_ms(40);
        drawNumber(resistor/1, 70, 130, ST7735_BLACK, ST7735_WHITE, 1);
        delay_ms(40);
        //drawNumber(error, 70, 50, ST7735_BLACK, ST7735_WHITE, 1);
        //delay_ms(40);

        if(ptvolt < ptvolt_high-20){
            relay_on(1);   // only nichrome wire on and fan is off
            drawString("increasing", 30, 50, ST7735_BLACK, ST7735_WHITE, 1);
            delay_ms(40);
        }else if((ptvolt_high-20) & (ptvolt < ptvolt_high)){   // nichrome wire and fan both off
            relay_on(2);   // nichrome wire in on and fan both on
            drawString("constant  ", 30, 50, ST7735_BLACK, ST7735_WHITE, 1);
            delay_ms(40);
        }else{
            relay_on(3);   // only fan is on, nichrome is off
            drawString("decreasing", 30, 50, ST7735_BLACK, ST7735_WHITE, 1);
            delay_ms(40);
        }
        delay_ms(500);
        time++;
    }
}



