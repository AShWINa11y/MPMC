/****************************************************
 * Read ADC channel 2 and display it on LCD Display
 ****************************************************/



#include "LPC214x.h"                       /* LPC21xx definitions  */

int ReadADC(char ChannelNumber);
void InitializeLCD(void);
void DisplayLCD(char LineNumber, char *Message);
void DisplayLCD2Digit(char LineNumber, char CharPosition, char Data);
void LCDDelay1600(void);

int main(void)
{  
    int a;              
    unsigned char Channel = 2;
    
    // Corrected: Bits 27:26 set to 01 for P0.29 as AD0.2
    PINSEL1 = 0x08000000;                   
    
    InitializeLCD();                        // Initialize LCD
    DisplayLCD(0, "   1st Batch   ");      // Display message
    DisplayLCD(1, "Channel 2:   ");      // Display message
    
    while(1)
    {
        a = ReadADC(Channel);               // Read ADC channel 2
        
        // Display high byte and low byte of the 10-bit result
        DisplayLCD2Digit(1, 10, (a >> 8) & 0xFF);   
        DisplayLCD2Digit(1, 12, (a & 0xFF));
        LCDDelay1600();
    }
}

// Read ADC data from given channel number dynamically
int ReadADC(char ChannelNumber)
{                     
    int val;
    
    // Setup A/D: 10-bit AIN, CLKDIV, Operational mode, and channel select
    // 0x00200000 enables the ADC (PDN bit)
    // 0x00010400 sets the clock divider
    AD0CR = 0x00200400 | (1 << ChannelNumber);             

    AD0CR |= 0x01000000;                    // Start A/D Conversion now (START = 001)

    do
    {
        val = AD0GDR;                       // Read Global Data Register dynamically
    }        
    while ((val & 0x80000000) == 0);        // Wait for DONE bit (bit 31) to go high
    
    AD0CR &= ~0x01000000;                   // Stop A/D Conversion 

    val = ((val >> 6) & 0x03FF);            // Extract 10-bit result (bits 15:6)
    return(val);                            // Return the Data Read
}
