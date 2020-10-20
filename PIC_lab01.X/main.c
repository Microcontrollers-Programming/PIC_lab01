/*
 * File:   main.c
 * Author: Advanced Digital Systems Laboratory
 *
 * Created on August 3, 2020, 11:57 AM
 */

// <editor-fold defaultstate="collapsed" desc="Configuration bits">

// CONFIG1
// External Oscillator Selection (Oscillator not enabled)
#pragma config FEXTOSC = OFF    

// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)
#pragma config RSTOSC = HFINTOSC_64MHZ

// CONFIG2
// Clock out Enable bit (CLKOUT function is disabled)
#pragma config CLKOUTEN = OFF
// PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be cleared and
// set only once)
#pragma config PR1WAY = ON
// Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed
// by user software)
#pragma config CSWEN = OFF
// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config FCMEN = OFF

// CONFIG3
// MCLR Enable bit
// (If LVP = 0, MCLR pin is MCLR;
// If LVP = 1, RE3 pin function is MCLR)
#pragma config MCLRE = EXTMCLR
// Power-up timer selection bits (PWRT is disabled)
#pragma config PWRTS = PWRT_OFF
// Multi-vector enable bit (Interrupt contoller does not use vector table
// to prioritze interrupts)
#pragma config MVECEN = OFF
// IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared
// and set only once)
#pragma config IVT1WAY = ON
// Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config LPBOREN = OFF
// Brown-out Reset Enable bits (Brown-out Reset disabled)
#pragma config BOREN = OFF

// CONFIG4
// Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR)
// set to 1.9V)
#pragma config BORV = VBOR_1P9
// ZCD Disable bit (ZCD module is disabled.
// ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config ZCD = OFF
// PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be cleared
// and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config PPS1WAY = ON
// Stack Full/Underflow Reset Enable bit (Stack full/underflow
// will cause Reset)
#pragma config STVREN = ON
// Low Voltage Programming Enable bit (HV on MCLR/VPP must be used
// for programming)
#pragma config LVP = OFF
// Extended Instruction Set Enable bit (Extended Instruction Set and
// Indexed Addressing Mode disabled)
#pragma config XINST = OFF

// CONFIG5
// WDT Period selection bits (Divider ratio 1:65536;
// software control of WDTPS)
#pragma config WDTCPS = WDTCPS_31
// WDT operating mode (WDT Disabled; SWDTEN is ignored)
#pragma config WDTE = OFF

// CONFIG6
// WDT Window Select bits (window always open (100%);
// software control; keyed access not required)
#pragma config WDTCWS = WDTCWS_7
// WDT input clock selector (Software Control)
#pragma config WDTCCS = SC

// CONFIG7
// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBSIZE = BBSIZE_512
// Boot Block enable bit (Boot block disabled)
#pragma config BBEN = OFF
// Storage Area Flash enable bit (SAF disabled)
#pragma config SAFEN = OFF
// Background Debugger (Background Debugger disabled)
#pragma config DEBUG = OFF

// CONFIG8
// Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTB = OFF
// Configuration Register Write Protection bit (Configuration registers
// not Write protected)
#pragma config WRTC = OFF
// Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTD = OFF
// SAF Write protection bit (SAF not Write Protected)
#pragma config WRTSAF = OFF
// Application Block write protection bit (Application Block not write
// protected)
#pragma config WRTAPP = OFF

// CONFIG10
// PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code
// protection disabled)
#pragma config CP = OFF

// </editor-fold>

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

// Δήλωση μεταβλητών για την αποθήκευση αθροίσματος και διαφοράς δύο αριθμών
uint8_t sum, difference;

// Δήλωση μεταβλητών για την αποθήκευση του κρατούμενοy εξόδου αθροίσματος
// και διαφοράς δύο αριθμών
bool carry_sum = 0, carry_difference = 0;

// Δήλωση μεταβλητών για την αποθήκευση του bit υπερχείλισης για το άθροισμα
// και τη διαφορά δύο αριθμών
bool overflow_sum = 0, overflow_difference = 0;

void main(void)
{
    // Καμία λειτουργία (no operation))
    asm("nop");
    
    // Καθορισμός θυρών PORTA, PORTC και PORTD ως ψηφιακές θύρες
    ANSELA = 0;
    ANSELC = 0;
    ANSELD = 0;
    
    // Καθορισμός ακροδεκτών RE1 και RE2 ως ψηφιακές είσοδοι/έξοδοι
    ANSELEbits.ANSELE1 = 0;
    ANSELEbits.ANSELE2 = 0;
    
    // Καθορισμός ακροδεκτη RB4 ως ψηφιακή είσοδος
    ANSELBbits.ANSELB4 = 0;
    
    // Καθορισμός θυρών PORTA και PORTC ως ψηφιακές είσοδοι
    TRISA = 0xFF;
    TRISC = 0xFF;
    
    // Καθορισμός θύρας PORTC ως ψηφιακή είσοδος
    TRISD = 0;
    
    // Καθορισμός ακροδέκτη RΒ4 ως ψηφιακή είσοδος
    TRISBbits.TRISB4 = 1;
    
    // Ενεργοποίηση της pull-up αντίστασης για τον ακροδέκτη RB4
    WPUBbits.WPUB4 = 1;
    
    // Καθορισμός ακροδεκτών RE1 και RE2 ως ψηφιακές έξοδοι
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    
    // Ατέρμων βρόχος
    for(;;)
    {
        // Καμία λειτουργία
        asm("nop");
        
        // Άθροισμα ψηφιακών λέξεων που
        // εφαρμόζονται στις θύρες PORTA και PORTC
        sum = PORTA + PORTC;
        // Αποθήκευση κρατουμένου αθροίσματος
        carry_sum = STATUSbits.C;
        // Αποθήκευση bit υπερχείλισης αθροίσματος
        overflow_sum = STATUSbits.OV;

        // Διαφορά ανάμεσα στις ψηφιακές λέξεις που
        // εφαρμόζονται στις θύρες PORTA και PORTC
        difference = PORTA - PORTC;
        // Αποθήκευση κρατουμένου διαφοράς
        carry_difference = STATUSbits.C;
        // Αποθήκευση bit υπερχείλισης διαφοράς
        overflow_difference = STATUSbits.OV;
        
        // Έλεγχος ψηφιακής εισόδου RB4
        // Αν RB4=1 τότε PORTD=άθροισμα
        // Αν RB4=0 τότε PORTD=διαφορά
        if (PORTBbits.RB4 == 1)
        {
            // Ανάθεση αθροίσματος στη
            // θύρα PORTD
            PORTD = sum;
            
            // Ανάθεση κρατουμένου αθροίσματος στον ακροδέκτη RE1
            PORTEbits.RE1 = carry_sum;
            
            // Ανάθεση bit υπερχείλισης στον ακροδέκτη RE2
            PORTEbits.RE2 = overflow_sum;
        }
        else
        {
            // Ανάθεση διαφοράς στη θύρα PORTD
            PORTD = difference;
            
            // Ανάθεση κρατουμένου διαφοράς στον ακροδέκτη RE1
            PORTEbits.RE1 = carry_difference;
            
            // Ανάθεση bit υπερχείλισης στον ακροδέκτη RE2
            PORTEbits.RE2 = overflow_difference;
        }
    }
    return;
}