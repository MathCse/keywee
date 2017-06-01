//TEST


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "general.h"
#include "lcd.h"

/******/
// CONFIG1H
#pragma config FOSC = INTIO7    // Oscillator Selection bits (Internal oscillator block, CLKOUT function on OSC2)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)


unsigned int steps = 0;

//TEST - TEST - TEST
    void show_debug(void) {
            LCDWriteInt(0, 1, steps);
    }

//TEST - TEST - TEST
    void init_debug(void) {
            LCDInit();
            LCDClear();
            LCDGoto(0, 0);
            LCDWriteStr("Nombre pas");
            show_debug();
    }

//Pr�pare les registres
    void setup(void) {
        //TEST - TEST - TEST
            init_debug();

        //Podom�tre (interruption prioritaire)
            INTCON2bits.INTEDG0 = 1; // Front montant
            INTCONbits.INT0IF = 0 ; //Nettoyage de flag
            TRISBbits.TRISB0 = 0; //Patte en sortie
            LATBbits.LATB0 = 0; //Nettoyage de la valeur
            TRISBbits.TRISB0 = 1; //Patte en entr�e
            ANSELBbits.ANSB0 = 0; //Entr�e en mode num�rique
            INTCONbits.INT0E = 1 ; //Activation de l'interruption

        //Mode recherche
            INTCON2bits.INTEDG1 = 1; // Front montant
            INTCON3bits.INT1IF = 0 ; //Nettoyage de flag
            TRISBbits.TRISB1 = 0; //Patte en sortie
            LATBbits.LATB1 = 0; //Nettoyage de la valeur
            TRISBbits.TRISB1 = 1; //Patte en entr�e
            ANSELBbits.ANSB1 = 0; //Entr�e en mode num�rique
            INTCON3bits.INT1E = 1 ; //Activation de l'interruption

        //Lampe-torche
            INTCON2bits.INTEDG2 = 1; // Front montant
            INTCON3bits.INT2IF = 0 ; //Nettoyage de flag
            TRISBbits.TRISB2 = 0; //Patte en sortie
            LATBbits.LATB2 = 0; //Nettoyage de la valeur
            TRISBbits.TRISB2 = 1; //Patte en entr�e
            ANSELBbits.ANSB2 = 0; //Entr�e en mode num�rique
            INTCON3bits.INT2E = 1 ; //Activation de l'interruption

        //Initialisation des pattes de sorties
            TRISCbits.RC1 = 0 ;
            LATCbits.LATC1 = 0 ;

        //Initialisation du Timer 2
            T2CONbits.T2OUTPS = 0 ; //Postscaler � 1
            T2CONbits.T2CKPS = 0b10 ; //Prescaler � 16
            T2CONbits.TMR2ON = 1 ; //Activation du timer

        //Initialisation de la PWM
            TRISCbits.RC2 = 0; //Patte en sortie
            CCP1CONbits.P1M = 0 ; //1 seule sortie
            CCP1CONbits.DC1B = 0 ; //LSB du rapport cyclique de la PWM
            CCP1CONbits.CCP1M = 0b1100 ; //Mode PWM
            CCPR1Lbits.CCPR1L = 0 ; //Mise � 0 de la PWM

        //Activation globales des interruptions
            INTCONbits.PEIE = 1; //Interruptions p�riph�riques activ�es
            INTCONbits.GIE = 1 ; //Interriptions activ�es

        //R�initialisaion du nombre de pas
            steps = 0;
    }

//Compte le nombre de pas depuis la derni�re synchronisation
    void pedometer() {
        //Incr�mentation du nombre de pas
            steps++;
        //Nettoyage du flag
            INTCONbits.INT0IF = 0 ; //Flag � 0

        //TEST - TEST - TEST
            show_debug();
    }

//Active ou d�sactive la lampe-torche
    void flashlight(void) {
        //Toggle de l'�tat de la lampe-torche
            LATCbits.LATC1 = (unsigned char) !LATCbits.LATC1; //Inversion de la sortie
        //Nettoyage du flag
            INTCON3bits.INT1IF = 0; //Flag � 0
    }

//Cri du kiwi ou darude sandstorm
//Fr�quence_PWM = (1MHz)/(4 * prescaler * (PR2+1))
    // Bass
    #define C3 118
    #define Cd3 111
    #define D3 105
    #define Ed3 99
    #define E3 94
    #define F3 88
    #define Fd3 83
    #define G3 79
    #define Gd3 74
    #define A3 70   //220
    #define Ad3 59
    #define B3 62

    // Medium
    #define C4 59
    #define Cd4 55
    #define D4 52
    #define Ed4 49
    #define E4 46
    #define F4 44
    #define Fd4 41
    #define G4 39
    #define Gd4 37
    #define A4 35   //440
    #define Ad4 33
    #define B4 31

    // Tremble
    #define C5 29
    #define Cd5 27
    #define D5 26
    #define Ed5 24
    #define E5 23
    #define F5 21
    #define Fd5 20
    #define G5 19
    #define Gd5 18
    #define A5 17   //880s
    #define Ad5 16
    #define B5 15

    /* DARUDE SANDSTORM - INTRO */
    /*unsigned char kiwi_length = 64;
    unsigned char kiwi_frequence[64] = {
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0,  0,  0,  0,  0,
                                         0,  0,  0,  0, 0, 0 ,  LA,  LA
                                       };
    */
    /* DARUDE SANDSTORM - RIFF */
    unsigned char kiwi_length = 128;
    unsigned char kiwi_frequence[128] = {
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0, B4,  0,  0,  0,
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0, E5,  0,  0,  0,
                                        E5,  0, E5,  0, E5,  0, E5,  0,
                                        E5,  0,  0,  0, D5,  0,  0,  0,
                                        D5,  0, D5,  0, D5,  0, D5,  0,
                                        D5,  0,  0,  0, A4,  0,  0,  0,
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0, B4,  0,  0,  0,
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0, E5,  0,  0,  0,
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0, B4,  0,  0,  0,
                                        B4,  0, B4,  0, B4,  0, B4,  0,
                                        B4,  0,  0,  0, E5,  0,  0,  0,
                                       };

    /*AU CLAIR DE LA LUNE by Adrien*/
       /* unsigned char kiwi_frequence[64] = {
                                        SOL, SOL, SOL, 0, SOL, SOL, SOL, 0,
                                        SOL, SOL, SOL, SOL, LA, LA, LA, LA,
                                        B4, B4, B4, B4, B4, B4, B4, B4,
                                        LA, LA, LA, LA ,LA, LA ,LA, LA,
                                        SOL, SOL, SOL, 0, B4, B4, B4, 0,
                                        LA, LA, LA, 0, LA, LA, LA, 0,
                                        SOL, SOL, SOL, SOL ,SOL, SOL ,SOL, SOL,
                                         0,  0,  0,  0,  0,  0,  0,  0

                                       }*/
    /*Mario main Theme by Adrien*/
    /*
        unsigned char kiwi_length = 340;
        unsigned char kiwi_frequence[340] = {
         E5, E5, E5, 0, E5, E5, E5, 0, E5, E5, E5, 0, DO, DO, DO, 0, E5, E5, E5, 0, SOL, SOL, SOL, 0, SOL, SOL, SOL, 0,
         DO, DO, DO, 0, SOL, SOL, SOL, 0, LA, LA, LA, 0, B4, B4, B4, 0, LA_d, LA_d, LA_d, 0, LA, LA, LA, 0, SOL, SOL, SOL, 0,
         E5, E5, E5, 0, SOL, SOL, SOL, 0, LA, LA, LA, 0, FA, FA, FA, 0, SOL, SOL, SOL, 0, E5, E5, E5, 0, DO, DO, DO, 0, D5, D5, D5, 0, B4, B4, B4, 0,
         DO, DO, DO, 0, SOL, SOL, SOL, 0, LA, LA, LA, 0, B4, B4, B4, 0, LA_d, LA_d, LA_d, 0, LA, LA, LA, 0, SOL, SOL, SOL, 0,
         E5, E5, E5, 0, SOL, SOL, SOL, 0, LA, LA, LA, 0, FA, FA, FA, 0, SOL, SOL, SOL, 0, E5, E5, E5, 0, DO, DO, DO, 0, D5, D5, D5, 0, B4, B4, B4, 0,
         SOL, SOL, SOL, 0, FA_d, FA_d, FA_d, 0, FA, FA, FA, 0, RE_d, RE_d, RE_d, 0, E5, E5, E5, 0,
         LA, LA, LA, 0, LA, LA, LA, 0, DO, DO, DO, 0, LA, LA, LA, 0, DO, DO, DO, 0, D5, D5, D5, 0,
         SOL, SOL, SOL, 0, FA_d, FA_d, FA_d, 0, FA, FA, FA, 0, RE_d, RE_d, RE_d, 0, E5, E5, E5, 0, DO, DO, DO, 0, DO, DO, DO, 0, DO, DO, DO, 0,
         SOL, SOL, SOL, 0, FA_d, FA_d, FA_d, 0, FA, FA, FA, 0, RE_d, RE_d, RE_d, 0, E5, E5, E5, 0,
         LA, LA, LA, 0, LA, LA, LA, 0, DO, DO, DO, 0, LA, LA, LA, 0, DO, DO, DO, 0, D5, D5, D5, 0,
         SOL, SOL, SOL, 0, FA_d, FA_d, FA_d, 0, FA, FA, FA, 0, RE_d, RE_d, RE_d, 0, E5, E5, E5, 0,
         LA, LA, LA, 0, LA, LA, LA, 0, DO, DO, DO, 0, LA, LA, LA, 0, DO, DO, DO, 0, D5, D5, D5, 0, DO_d, DO_d, DO_d, 0, D5, D5, D5, 0, DO, DO, DO, 0
    };*/


//Mode recherche
    void searched(void) {
        //Dur�e maximale de la recherche (environ, en secondes)
            int time = 0, max_time = 500, i = 0;

        //Mode recherche
            while (time++ < max_time) {
                //Modification de la PWM
                    PR2bits.PR2 = kiwi_frequence[time%kiwi_length]; //Registre de p�riode pour le timer 2
                    CCPR1Lbits.CCPR1L = (unsigned char) ((kiwi_frequence[time%kiwi_length] + 1)/2) ; //Rapport cyclique
                //Clignotement LED
                    flashlight();
                     _delay(15000);
                //Temporisation et si le bouton est appuy�, quitter la boucle
                    /*for (i = 0; i < 1; i++) {
                        if (INTCON3bits.INT2IF) { time = max_time; break ; }
                       _delay(25000);
                       _delay(25000);
                    }*/
            }

        //D�sactivation de la PWM et de la lampe-torche
            CCPR1Lbits.CCPR1L = 0 ; //Mise � 0 de la PWM
            LATCbits.LATC1 = 0; //D�sactivation forc�e de la lampe-torche
            INTCON3bits.INT2IF = 0; //Nettoyage du flag bouton
    }


//Fonction de synchronisation des donn�es avec la t�l�commande
    void synchronize(void) {
        //TODO - Transmission du nombre de pas parcouru
        //R�initialisation du compteur de pas
            steps = 0;
    }

//Traitement des interruptions
    void interrupt high_priority ISR(void){
        //Traitement de l'interruption
            if (INTCONbits.INT0IF) { pedometer(); } //Podom�tre
            if (INTCON3bits.INT1IF) { searched(); } //Mode recherche
        //Lampe-torche et synchronisation
            if (INTCON3bits.INT2IF) {
                //D�but du comptage de la dur�e de l'appui sur le bouton
                    int i = 0;
                    for (i = 0; i < 30; i++) {
                        if (!LATBbits.LATB2) {  break ; }
                        _delay(25000);
                    }
                //Appui long
                    if (i == 30) { synchronize(); }
                //Appui court
                    else { flashlight();  }
            }
    }

//Boucle principale
    int main(int argc, char** argv) {
        //R�glages
            setup();
        //Veille
            while (1) { }
        //Fin de programme
            return (EXIT_SUCCESS);
    }
