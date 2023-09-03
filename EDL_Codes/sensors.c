
#include "myLib.h"
//----------------------------------------
// Prototypes
//----------------------------------------

// TO STORE THE VALUE IN VARIABLE ADC0Value FOR EVERY SAMPLING
uint32_t ADC0Value[1];

//---------------------------------------------------------------------------
// hardware_init()
//---------------------------------------------------------------------------
void relay_init(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

// (pin PD0)-> nichrome wire,
// (pin PD1)-> fan\
// 1 -> only nichrome wire on and fan is off
// 2 -> nichrome wire in on and fan both on
// 3 -> only fan is on, nichrome is off
void relay_on(int number){
    if(number == 1){
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x01);
    }else if(number == 2){
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x03);
    }else if(number == 3){
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x02);
    }
}

void adc_init(void)
{
    //Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
    //SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // ADD Tiva-C GPIO setup -
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // ENABLE ADC0 MODUL
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // ENABLE GPIO for ADC0 MODULE

    GPIOPinTypeADC(PORT_E,GPIO_PIN_3);// ENABLE AN0 OF ADC0 MODULE
    // ADC0 MODULE, TRIGGER IS PROCESSOR EVENT, SEQUENCER 0 IS CONFIGURED
    ADCSequenceConfigure(ADC_0, 1, ADC_TRIGGER_PROCESSOR, 0);

    // ADC0 MODULE, SEQUENCER 0 , FOR 1 SAMPLING, INPUT IS FROM CHANNEL 0 PE3
    ADCSequenceStepConfigure(ADC_0, 1, 0, ADC_CTL_CH0);
    // ENABLE THE SEQUENCE 1 FOR ADC0
    ADCSequenceEnable(ADC_0, 1);
}

int ADC_conv(void)
{
    // CLEAR INTERRUPT FLAG FOR ADC0, SEQUENCER 1
    ADCIntClear(ADC_0, 1);
    // TRIGGER IS GIVEN FOR ADC 0 MODULE, SEQUENCER 1
    ADCProcessorTrigger(ADC_0, 1);
    // STORE THE CONVERTED VALUE FOR ALL DIFFERENT SAMPLING IN ARRAY
    //ui32ADC0Value
    ADCSequenceDataGet(ADC_0, 1, ADC0Value);
    return ADC0Value[0];
}

float volt_to_resistor(ptvolt){
    float res_side = 120;
    float v_pow = 3.3;
    float v_adc = ptvolt*v_pow/4096;
    float v_high = 5;
    float resistor = res_side*v_adc*(v_high-v_adc);
    return v_adc*100;
}

int resistor_to_temp(float resistor){
    // 163 high high very
    // 160 la band karo -> 250 degree
    // 112 room temperature 28 degree
    int high_temp = 250;
    int room_temp = 28;
    int res_high = 160;
    int res_normal = 112;
    int temp;
    temp = 28+(resistor-res_normal)*(high_temp-room_temp)/(res_high-res_normal);
    return temp;
}
