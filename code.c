#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#define _XTAL_FREQ 20000000
//#include <math.h>
short count=2;
char val;
char cTrl=1;
short check=1;
short in = 0;
short str[3];
short  Ginput=0;
short  Yinput=0;
void uart_init(int baudrate){
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    SPBRG = ((_XTAL_FREQ/16)/baudrate) - 1;
    TXSTA = 0B00100100;
    RCSTA = 0B10010000;
}
void uart_tx(char ch){
    while(!TXIF);
    TXREG = ch;
}
char uart_rx(){
    if(OERR){//==1
        CREN = 0;
        CREN = 1;
    }
    while(!RCIF);
    return RCREG;
}
void uart_sent(char *p){
    while(*p){
        uart_tx(*p++);
    }
}
void init () //setup pin
{     
    TRISB = 0b00000000;
    PORTB = 0b00000000;  
    TRISCbits.TRISC0=1;
    TRISCbits.TRISC1=1;
    TRISCbits.TRISC2=1;
    TRISD = 0b00000000; 
    PORTD = 0b00000000;
    PIE1bits.RCIE = 1; // Kích ho?t ng?t RDA
    INTCONbits.PEIE = 1; // Kích ho?t ng?t ngo?i vi
    INTCONbits.GIE = 1; // Kích ho?t ng?t toàn c?c  
    TXSTAbits.TXEN = 1; // B?t ch? ?? truy?n
    RCSTAbits.SPEN = 1; // B?t c?ng n?i ti?p
    RCSTAbits.CREN = 1; // B?t ch? ?? nh?n
}
void clearScreen()
{    while (!TXIF); // Ch? ??n khi thanh ghi truy?n s?n sàng
    TXREG = 0x0C; // G?i ký t? ASCII c?a l?nh CLS (0x0C) 
}
int number[10]={
    0b11111100, //0
    0b01100000, //1
    0b11011010, //2
    0b11110010, //3
    0b01100110, //4
    0b10110110, //5
    0b10111110, //6
    0b11100000, //7
    0b11111110, //8
    0b11110110, //9  
};
void lat(){//Latch data
    // Latch: RB2
    // clock :RB0
    RB2 = 1;
    __delay_us(200);
    RB2 = 0; 
}
void nUm_OUT(int  num){ //d?ch bit Light_1,3
  // Latch: RB2
   // clock :RB0
   // Data:RB1
    for (int i = 0;i<8;i++)
    {     
        RB1 = (num >> i)&0b00000001;
     
        RB0 = 1;
        __delay_us(200);
        RB0 = 0;
        __delay_us(200); 
    }
}
void nUm_OUT2(int num){//d?ch bit Light_2,4
  // Latch: RB2
   // clock :RB0
   // Data:RB1
    for (int i = 0;i<8;i++)
       { RB3 = (num >> i)&0b00000001;
        RB0 = 1;
        __delay_us(200);
        RB0 = 0;
        __delay_us(200);   }
}
void hold(){
    nUm_OUT(0b00000000);
    nUm_OUT(0b00000000);
    lat();
     __delay_us(200);
}
void seg( int input ) // out_7Seg  Light_1,3
{    // number of wait time 
    short ngan;
    short tram; 
    short chuc;
    short donvi;
     ngan=input/1000;
     tram=(input/100)%10;
     chuc=(input%100)/10;
     donvi=(input%10);    
           nUm_OUT( 0b00000000);
           nUm_OUT( 0b01110000); 
           nUm_OUT( number[ngan]);  
           lat();           
        hold();
           nUm_OUT( 0b10110000); 
           nUm_OUT( number[tram]);  
           lat();          
        hold();
           nUm_OUT( 0b11010000); 
           nUm_OUT( number[chuc]);  
           lat();         
        hold();
           nUm_OUT( 0b11100000); 
           nUm_OUT( number[donvi]);  
           lat();        
        hold();    
}
void seg2( int input ) // out_7Seg Light_2,4
{    // number of wait time 
    short ngan;
    short tram; 
    short chuc;
    short donvi;
     ngan=input/1000;
     tram=(input/100)%10;
     chuc=(input%100)/10;
     donvi=(input%10);   
           nUm_OUT2( 0b00000000);
           nUm_OUT2( 0b01110000); 
           nUm_OUT2( number[ngan]);  
           lat();           
        hold();
           nUm_OUT2( 0b10110000); 
           nUm_OUT2( number[tram]);  
           lat();          
        hold();
           nUm_OUT2( 0b11010000); 
           nUm_OUT2( number[chuc]);  
           lat();         
        hold();
           nUm_OUT2( 0b11100000); 
           nUm_OUT2( number[donvi]);  
           lat();  
        hold();        
}
int pOw(short number) // exponent
{    short result = 1.0;
     for (int i = 0; i < number; i++) 
            result *= 10;
    return result;
}
void input(int count )
{   TXREG= '\r';
    uart_sent(" Enter the time for the green light: ");     
      // input Red light      
           while(count != -1)
           {   str[count]=(int)uart_rx() - 48;
               Ginput+=(str[count]*pOw(count) );
               count--;       }
    uart_sent(" second ");       
    count=2;
    TXREG= '\r';
    uart_sent(" Enter the time for the Yellow light: ");
        // input Yellow light     
            while(count != -1)
           {   str[count]=(int)uart_rx() - 48;
               Yinput+=(str[count]*pOw(count));
               count--;           }
           uart_sent(" second ");       
    TXREG= '\r';
    TXREG= '\r';
    uart_sent(" #Press C to change time ");
    TXREG= '\r';
    uart_sent(" #Press M to change mode ");
}
void __interrupt() isr_uart(void)
{  
    if (PIR1bits.RCIF) // Ki?m tra c? ng?t RDA
    { if( RCREG == 'M')
      { uart_sent("  Mode:  ");
        TXREG= '\r';
        uart_sent(" -press X for Automatic counter  ");  
        TXREG= '\r';
        uart_sent(" -press O for Controller  ");  
        TXREG='\r';
       } 
        if( RCREG == 'X')
        {   TXREG= '\r';
            cTrl = '0';
             val='X';
            clearScreen();
            uart_sent("  ---AutoMode---   "); 
              input( count );     
        }
       
     if ( RCREG == 'C' )     
               {   clearScreen();
                    uart_sent("  **Change  ");
                    TXREG= '\r';
                    val='X';
                    cTrl = '0';
                    Ginput=0;
                    Yinput=0;
                     input(count);
               }
         if( RCREG == 'O' )
        {  TXREG= '\r';
           cTrl = '1';
           val='O';
         
           clearScreen();
           
            uart_sent("  ---ControlMode---  ");
            uart_sent("CHON CHE DO ");
                
            
            }
             if (RCREG == 'D' && cTrl == '1' ) 
             { uart_sent("CHE DO DEN 13 XANH ");  val = 'D';  }
             if (RCREG == 'A' && cTrl == '1') 
             { uart_sent("CHE DO DEN 24 XANH ");  val = 'A';  }
            PIR1bits.TXIF = 0; // Xóa c? ng?t TXIF
            PIR1bits.RCIF = 0; // Xóa c? ng?t RDA
    }
}

void main(void) {
     init(); 
     uart_init(9600);
//     Timer_Configuaration();
     char rc=1;
     char gc=1;
     char yc=1;
     char nc=1;
     
   uart_sent("Press M");
     
   while(1)
    {     
    
     
         if( val == 'X' ) //Auto
          {   
               
              
                        
                   short Green =Ginput;
                   short Yellow =Yinput;     
                   
                   short Red=Ginput+Yinput;
                   
                    while( Green != 0) // turn on green light
                   {
                      
                        
                        RD0=gc; //Green _ light 1,3
                        RD1=0 ; //Yellow _ light 1,3
                        RD2=!rc; //Red _ light 1,3
                        RD3= !gc; //Green _ light 2,4
                        RD4= 0; //Yellow _ light 2,4
                        RD5=rc; //RED _ light 2,4                   
                  
                        if( nc == 1)
                        {
                            seg(Green );
                            seg2(Red);  
                        }
                         if (nc == 0)
                        {
                         seg(Red); 
                         seg2(Green);
                        
                       

                        }   
                          Green--;
                          Red--;
                    __delay_ms(950);
                   }      
              
                   while( Yellow != 0) // turn on Yellow light
                   { 
                         
                       
                            RD0=0; //Green _ light 1,3
                            RD1=yc ; //Yellow _ light 1,3
                            RD2=!rc; //Red _ light 1,3
                            RD3= 0; //Green _ light 2,4
                            RD4= !yc; //Yellow _ light 2,4
                            RD5=rc; //RED _ light 2,4                
                                if( nc == 1)
                               { 
                                seg(Yellow); 
                                seg2(Red);
                               }  
                               if (nc == 0)
                                {
                                seg(Red); 
                                seg2(Yellow);                               

                                }   
                            
                                 Yellow--;
                                 Red--;
                                __delay_ms(950);
                   } 
                   
                        
                rc=!rc;
                gc=!gc;    
                yc=!yc; 
                nc=!nc;        
                
         }        
        if(val == 'D')// uart_sent("CHE DO DEN 13 XANH ");
        {  
            RD1 = 0 ;
            RD2 = 0;
            RD3 = 0;
            RD4 = 0;            
        if( in == 0)
           { RD1 = 1;RD4 = 1; __delay_ms(1000);  in = 1; }
          RD5 = 1;  
          RD0 = 1 ;   
         
        }
        if(val == 'A')//uart_sent("CHE DO DEN 24 XANH ");
         
        { RD0 = 0 ;
          RD1 = 0 ; 
          RD4 = 0;
          RD5 = 0;
           
            if( in == 1 || check == 1)
           { RD1 = 1;RD4 = 1; __delay_ms(1000); in =0;check =0;}
            
           RD2 = 1;
           RD3 = 1;
          
        }
 }  
    return;
}
