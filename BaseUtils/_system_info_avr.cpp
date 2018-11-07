#include "dbgutils.h"
#ifdef __AVR__

/*
    This works on Arduinos with a 328 or 168 only
    The voltage is returned in millivolts. So 5000 is 5V, 3300 is 3.3V.
*/
float getVcc()
{
    long result; // Read 1.1V reference against AVcc 
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
    delay(2); // Wait for Vref to settle 
    ADCSRA |= _BV(ADSC); // Convert 
    while (bit_is_set(ADCSRA,ADSC)); 
    result = ADCL; 
    result |= ADCH<<8; 
    result = 1126400L / result; // Back-calculate AVcc in mV return result;
    //The voltage is returned in millivolts. So 5000 is 5V, 3300 is 3.3V.

    return (float)result / 1000.0; 
}

/*
    This works on Arduinos using CPU's with '8P' 
    Temperature is °C. 
*/
float readTemp() 
{ 
    long result; // Read temperature sensor against 1.1V reference 
    ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3); delay(2); // Wait for Vref to settle 
    ADCSRA |= _BV(ADSC); // Convert 
    while (bit_is_set(ADCSRA,ADSC)); 
    result = ADCL; 
    result |= ADCH<<8; 
    result = (result - 125) * 1075; //Temperature is returned in milli-°C. So 25000 is 25°C.

    return (float)result / 1000.0; 
}


#endif