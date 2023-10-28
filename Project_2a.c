#include <stdint.h>
#include <stdio.h>

/// Create definition for PORT B registers
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

//Create definitions for Port E registers
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

/// ADC
#define SYSCTL_RCGCADC_R        (*((volatile unsigned long *)0x400FE638))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
#define ADC0_ACTSS_R            (*((volatile unsigned long *)0x40038000))
#define ADC0_EMUX_R             (*((volatile unsigned long *)0x40038014))
#define ADC0_SSMUX3_R           (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R           (*((volatile unsigned long *)0x400380A4))
#define ADC0_PSSI_R             (*((volatile unsigned long *)0x40038028))
#define ADC0_RIS_R              (*((volatile unsigned long *)0x40038004))
#define ADC0_SSFIFO3_R          (*((volatile unsigned long *)0x400380A8))
#define ADC0_ISC_R              (*((volatile unsigned long *)0x4003800C))

#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_RIS_R            (*((volatile unsigned long *)0x4003001C))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define SYSCTL_RCGCTIMER_R      (*((volatile unsigned long *)0x400FE604))

//Create definition for Port F
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

void ADC_Init(void);
void timer0A_delayMs(int ttime);
void timer0_InIt(void);
void PortBE_Init(void);
void PortF_Init(void);
void Display(int digit, int number);
void NumSplit(int counted);
int numbers[10] = {0x40,0x79,0x24,0x30,0x19, // Each value turns on bits needed
0x12,0x02,0x78,0x00,0x10}; // to show numbers in display
int digit1, digit2, digit3, digit4; // Number to be displayed in each digit

void timer0_InIt(void){
SYSCTL_RCGCTIMER_R |= 0x01;     /* enable clock to Timer0 */

    TIMER0_CTL_R = 0x00;         /* disable Timer before initialization */
    TIMER0_CFG_R = 0x04;         /* 16-bit option */
    TIMER0_TAMR_R = 0x02;        /* periodic mode and down-counter */
    TIMER0_TAILR_R = 16000 - 1;  /* Timer A interval load value register */
    TIMER0_ICR_R = 0x1;          /* clear the TimerA timeout flag*/
    TIMER0_CTL_R |= 0x01;        /* enable Timer A after initialization */
TIMER0_TAPR_R = 1; // Prescalar value.. Can extend the cycle time max 256 times
}

void ADC_Init(void){
SYSCTL_RCGC2_R |= 0x08;
SYSCTL_RCGCADC_R |= 0x01;   // Enable clock ADC0
/* initialize PD0 for AIN0 input */
  GPIO_PORTD_AFSEL_R |= 0x1;        /* enable alternate function */
  GPIO_PORTD_DEN_R &= ~0x1;         /* disable digital function */
  GPIO_PORTD_AMSEL_R |= 0x1;        /* enable analog function */

/* initialize ADC0 */
  ADC0_ACTSS_R &= ~0x8;             /* disable SS3 during configuration */
  ADC0_EMUX_R &= ~0xF000;         /* software trigger conversion */
  ADC0_SSMUX3_R = 0x5;              /* get input from channel 0 */
  ADC0_SSCTL3_R |= 0x6;             /* take one sample at a time, set flag at 1st sample */
  ADC0_ACTSS_R |= 0x8;              /* enable ADC0 sequencer 3 */
}

// Subroutine to initialize ports B, E
void PortBE_Init(void){
  SYSCTL_RCGC2_R |= 0x00000002;     // Port B clock initialized
  GPIO_PORTB_CR_R = 0x7F;           // Allow changes to PB6-PB0
  GPIO_PORTB_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;   // GPIO clear bit PCTL
  GPIO_PORTB_DIR_R = 0x7F;          // Set PB6-PB0 outputs
  GPIO_PORTB_PUR_R = 0x00;          // Enable pullup resistors on PB4,PF0
  GPIO_PORTB_DEN_R = 0x7F;          // 7) Enable digital pins PB6-PB0

  SYSCTL_RCGC2_R |= 0x00000010;     // Port E clock initialized
  GPIO_PORTE_CR_R = 0x0F;           // Allow changes to PE4-0
  GPIO_PORTE_AMSEL_R = 0x00;        // Disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // GPIO clear bit PCTL
  GPIO_PORTE_DIR_R = 0x0F;          // PE3-PE0 output
  GPIO_PORTE_PUR_R = 0x00;          // Disable pullup resistors
  GPIO_PORTE_DEN_R = 0x0F;          // Enable digital pins PE3-PE0
}

void PortF_Init(void){ volatile unsigned long d;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  d = SYSCTL_RCGC2_R;               //Dummy instruction to wait for clock to stabilize
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0
}

int main(){

    int temperature, i, x=0;
    PortBE_Init();
    ADC_Init();
    PortF_Init();

    while(1){
        /* wait for conversion to complete */
        GPIO_PORTF_DATA_R = 0x02;
		if(x==50){
            ADC0_PSSI_R |= 0x08;
            while((ADC0_RIS_R & 8)== 0x0);

            if(((ADC0_SSFIFO3_R & 0xFFF) < 2300) ){
               temperature = ((ADC0_SSFIFO3_R & 0xFFF)* 165/2300) - 70;
            }
            if((ADC0_SSFIFO3_R & 0xFFF) > 2300){
               GPIO_PORTF_DATA_R = 0x08;
               timer0A_delayMs(1000);
            }
            x=0;
            NumSplit(temperature);
            ADC0_ISC_R |= 0x8;
            timer0A_delayMs(2);
        }

        x++;

        // Display(1,digit4); // Display number for lowest digit
        timer0A_delayMs(2);
        Display(2,digit3);
        timer0A_delayMs(2);
        Display(4,digit2);
        timer0A_delayMs(2);
        Display(8,digit1);
        timer0A_delayMs(2);
    }
}

void Display(int digit, int number){
    GPIO_PORTB_DATA_R = 0x00; // Turns off LEDs
    GPIO_PORTE_DATA_R = digit; // Selects digit
    GPIO_PORTB_DATA_R = numbers[number]; // Turns on number in selected digit
    int i;
    timer0A_delayMs(2);
}

// Splits number in counter into 4 separate numbers for each digit
void NumSplit(int counted){
    digit1 = counted%10; //Copies value in counter, divides it by 10 and then keeps remainder
    counted /= 10; //Dividing value in counter by 10 shifts it by one decimal
    digit2 = counted%10;
    counted /= 10;
    digit3 = counted%10;
    counted /= 10;
    digit4 = counted%10; // digit1: holds 1000's digit, digit2: 100's digit,
    counted /=10; // digit3: 10th digit, digit4: unit digit
}

void timer0A_delayMs(int ttime)
{
    timer0_InIt();
    int i;
    for(i = 0; i < ttime; i++) {
        while ((TIMER0_RIS_R & 0x01) == 0);      /* wait for TimerA timeout flag */
        TIMER0_ICR_R = 0x01;      /* clear the TimerA timeout flag */
    }
}
