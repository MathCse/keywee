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
unsigned char synchronization = 0;

//TEST - TEST - TEST
    void show_debug(void) { 
            LCDWriteInt(0, 1, steps); 
            if (synchronization) {
                LCDGoto(8, 1); 
                LCDWriteStr("Sync");
            }
    }

//TEST - TEST - TEST
    void init_debug(void) {
            LCDInit();
            LCDClear();
            LCDGoto(0, 0); 
            LCDWriteStr("Nombre pas");
            show_debug();
    }

//Prépare les registres
    void setup(void) {   
        //TEST - TEST - TEST
            init_debug(); 

        //Podomètre (interruption prioritaire)
            INTCON2bits.INTEDG0 = 0; // Front montant
            INTCONbits.INT0IF = 0 ; //Nettoyage de flag
            TRISBbits.TRISB0 = 0; //Patte en sortie
            LATBbits.LATB0 = 0; //Nettoyage de la valeur
            TRISBbits.TRISB0 = 1; //Patte en entrée
            ANSELBbits.ANSB0 = 0; //Entrée en mode numérique
            INTCONbits.INT0E = 1 ; //Activation de l'interruption

        //Mode recherche
            INTCON2bits.INTEDG1 = 0; // Front montant
            INTCON3bits.INT1IF = 0 ; //Nettoyage de flag
            TRISBbits.TRISB1 = 0; //Patte en sortie
            LATBbits.LATB1 = 0; //Nettoyage de la valeur
            TRISBbits.TRISB1 = 1; //Patte en entrée
            ANSELBbits.ANSB1 = 0; //Entrée en mode numérique
            INTCON3bits.INT1E = 1 ; //Activation de l'interruption

        //Lampe-torche
            INTCON2bits.INTEDG2 = 1; // Front montant
            INTCON3bits.INT2IF = 0 ; //Nettoyage de flag
            TRISBbits.TRISB2 = 0; //Patte en sortie
            LATBbits.LATB2 = 0; //Nettoyage de la valeur
            TRISBbits.TRISB2 = 1; //Patte en entrée
            ANSELBbits.ANSB2 = 0; //Entrée en mode numérique
            INTCON3bits.INT2E = 1 ; //Activation de l'interruption
            
        //Initialisation des pattes de sorties
            TRISCbits.RC1 = 0 ;
            LATCbits.LATC1 = 0 ;

        //Initialisation du Timer 2
            T2CONbits.T2OUTPS = 0 ; //Postscaler à 1
            T2CONbits.T2CKPS = 0b10 ; //Prescaler à 16
            T2CONbits.TMR2ON = 1 ; //Activation du timer

        //Initialisation de la PWM
            TRISCbits.RC2 = 0; //Patte en sortie
            CCP1CONbits.P1M = 0 ; //1 seule sortie
            CCP1CONbits.DC1B = 0 ; //LSB du rapport cyclique de la PWM
            CCP1CONbits.CCP1M = 0b1100 ; //Mode PWM
            CCPR1Lbits.CCPR1L = 0 ; //Mise à 0 de la PWM

        //Activation globales des interruptions
            INTCONbits.PEIE = 1; //Interruptions périphériques activées
            INTCONbits.GIE = 1 ; //Interriptions activées
            
        //Réinitialisaion du nombre de pas
            steps = 0;
    }

//Compte le nombre de pas depuis la dernière synchronisation
    void pedometer() {
        //Incrémentation du nombre de pas
            steps++;
        //Nettoyage du flag
            INTCONbits.INT0IF = 0 ; //Flag à 0
        
        //TEST - TEST - TEST
            show_debug(); 
    }

//Active ou désactive la lampe-torche
    
    void flashlight(void) {
        //Toggle de l'état de la lampe-torche
            LATCbits.LATC1 = (unsigned char) !LATCbits.LATC1; //Inversion de la sortie
        //Nettoyage du flag
            INTCON3bits.INT2IF = 0; //Flag à 0
    }
    
//Cri du kiwi ou darude sandstorm
//Fréquence_PWM = (1MHz)/(4 * prescaler * (PR2+1))
    #define DO 59
    #define DO_d 55
    #define RE 52
    #define RE_d 49
    #define MI 46
    #define FA 44
    #define FA_d 41
    #define SOL 39
    #define SOL_d 37
    #define LA 35 
    #define LA_d 33
    #define SI 31
    unsigned char kiwi_length = 3;
    unsigned char kiwi_frequence[3] = {LA, LA, LA};
    
//Mode recherche
    void searched(void) {   
        //Durée maximale de la recherche (environ, en secondes)
            int time = 50, i = 0, found = 0;
            
        //Mode recherche
            while ((time-- > 0)&&(!found)) {
                //Modification de la PWM
                    PR2bits.PR2 = kiwi_frequence[time%kiwi_length]; //Registre de période pour le timer 2
                    CCPR1Lbits.CCPR1L = (unsigned char) ((kiwi_frequence[time%kiwi_length] + 1)/2) ; //Rapport cyclique
                //Clignotement LED
                    flashlight();
                //Temporisation et si le bouton est appuyé, quitter la boucle
                    for (i = 0; i < 10; i++) { 
                        if (INTCON3bits.INT2IF || LATBbits.LATB2) { found = 1; break ; } 
                        _delay(25000);
                        if (i > 5) { CCPR1Lbits.CCPR1L = 0; }
                    }
            }
            
        //Désactivation de la PWM et de la lampe-torche
            CCPR1Lbits.CCPR1L = 0 ; //Mise à 0 de la PWM
            LATCbits.LATC1 = 0; //Désactivation forcée de la lampe-torche
            INTCON3bits.INT1IF = 0; //Nettoyage du flag bouton
            INTCON3bits.INT2IF = 0; //Nettoyage du flag bouton
    }
    
//Fonction de synchronisation des données avec la télécommande
    void synchronize(void) {
        //TODO - Transmission du nombre de pas parcouru
            synchronization = 1;
        //TEST - TEST - TEST
            show_debug();
        //TEST - TEST - TEST
            _delay(625000);
        //Réinitialisation du compteur de pas
            steps = 0;
        //Fin de synchronisation
            synchronization = 0;
            INTCON3bits.INT2IF = 0; //Nettoyage du flag bouton
        //TEST - TEST - TEST
            show_debug();
            LCDGoto(8, 1);
            LCDWriteStr("    ");
    }

//Traitement des interruptions
    void interrupt high_priority ISR(void){ 
        //Traitement de l'interruption
            if (INTCONbits.INT0IF) { pedometer(); } //Podomètre
            if (INTCON3bits.INT1IF) { searched(); } //Mode recherche
        //Lampe-torche et synchronisation
            if (INTCON3bits.INT2IF) {
                //Début du comptage de la durée de l'appui sur le bouton
                    int i = 0;
                    _delay(75000);
                    for (i = 3; i < 30; i++) { 
                        LCDWriteInt(8, 1, i);
                        if (!PORTBbits.RB2) {  break ; } 
                        _delay(25000);
                    }
                //TEST - TEST - TEST
                    LCDGoto(8, 1);
                    LCDWriteStr("    ");
                //Appui long
                    if (i == 30) { synchronize(); } 
                //Appui court
                    else { flashlight();  }   
            }
    }

//Boucle principale
    int main(int argc, char** argv) {
        //Réglages
            setup();
        //Veille
            while (1) { }
        //Fin de programme
            return (EXIT_SUCCESS);
    }