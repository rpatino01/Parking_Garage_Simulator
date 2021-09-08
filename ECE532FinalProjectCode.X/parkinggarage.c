/*************************************************
 * File:   parkinggarage.c  - Rich Patino - 5/6/21
 * Final Project - University of Hartford - ECE532 
 ************************************************/
#include "ECE532_includes_v00.h"

char key; //declare character
int vehicle = 0; //declare counter

//Username
char User1, User2, User3,EnterUser;
int UserA = 0, UserB = 0, UserC = 0;
//Password
char Pass1, Pass2, Pass3, Pass4, Pass5, EnterPassword;

//define keypad
#define Keypad_PORT   TRISC
//define LCD pins
#define RS LATD0 /* PORTB 1 pin is used for Register Select */
#define EN LATD1 /* PORTD 1 pin is used for Enable */
#define ldata LATB /* PORTB is used for transmitting data to LCD */
#define LCD_Port TRISB
#define LCD_Control TRISD

#define NUM_LINES 2 // Number of display lines
#define NUM_COL 16 // Number of characters per line
//Function Declarations
void LCD_Init();   //Declares Function
void LCD_Command(char ); //Declares Function
void LCD_Char(char x); //Declares Function
void LCD_String(const char *); //Declares Function
void LCD_Clear(); //Declares Function
void LCD_GoTo(char column,char line); //Declares Function
void Ask_User(); //Declares Function
void Ask_Password(); //Declares Function


char GetKey(void); //Declares Function
char keypad_scanner(void); //Declares Function

void Keypad_Init(void);

void main()
{
    OSCCON = 0x72; //internal Oscillator
    Keypad_Init();  //initialize keypad
    LCD_Init(); /* Initialize 16x2 LCD */
    TRISA = 0x00; //port A as output
    
  LCD_String("Sands Parking"); //Print Message to LCD
   __delay_ms(4000); //delay of 4 sec
   LCD_Clear(); //clear display

   while(1)
   {
      LCD_Init(); //Initialize the LCD Display
      //New Car Arrives
       if(PORTDbits.RD2 == 1) 
       {
          if(vehicle == 10) //Display when parking lot is full
          {
              LCD_String("Lot Full"); //Print Message to LCD
              __delay_ms(2000); //delay of 2 sec
          }
          else
          {  //Rest of code goes here
            
              Ask_User(); //Call Function
              __delay_ms(2000); //delay of 2 sec
              Ask_Password(); //Call Function
             
          }
       }
//       //exit
       if(PORTDbits.RD3 == 1)
       {
        LCD_String("Exit Gate Open"); //Print Message to LCD
         RA1 = 1; //Turn on LED 
         __delay_ms(2000); //delay of 2 sec
         vehicle--; //Decrease vehicle counter by 1  
         RA1 = 0; //Turn off LED
       }
       //emergency
       if(PORTDbits.RD4 == 1)
       {
        RA1 = 1; //exit door led on
          
          LCD_String("Emergency"); //Print Message to LCD
          for(int i =0; i < 10; i++) //led flashes
          {
               
            RA2 = 1; //Turn on LED
            __delay_ms(150); //Delay for 150ms
            RA2 = 0; //Turn on LED 
            __delay_ms(150); //Delay for 150ms
            
          } 
          RA1 = 0; //exit door led off       
         vehicle = 0; //Set Vehicle counter to 0
       }
     
  
    }         
}
      
           

char GetKey(void)   //Function Gets Key input from the User
{
    char key = 'n'; //No key pressed
    while(key == 'n')  //Wait till a key is pressed
    key = keypad_scanner(); // Scan the keys again and again
    return key;  //when key pressed then return its value
}

char keypad_scanner(void)  
    {           
            //Scan Keypad ROW1
            PORTCbits.RC0 = 0; PORTCbits.RC1 = 1; PORTCbits.RC2 = 1; PORTCbits.RC3 = 1;    
            if (PORTCbits.RC4 == 0) { __delay_ms(100); while (PORTCbits.RC4==0); return '*'; }
            if (PORTCbits.RC5 == 0) { __delay_ms(100); while (PORTCbits.RC5==0); return '2'; }
            if (PORTCbits.RC6 == 0) { __delay_ms(100); while (PORTCbits.RC6==0); return '3'; }
           
            //Scan Keypad ROW2
             PORTCbits.RC0 = 1; PORTCbits.RC1 = 0; PORTCbits.RC2 = 1; PORTCbits.RC3 = 1;    
            if (PORTCbits.RC4 == 0) { __delay_ms(100); while (PORTCbits.RC4==0); return '1'; }
            if (PORTCbits.RC5 == 0) { __delay_ms(100); while (PORTCbits.RC5==0); return '5'; }
            if (PORTCbits.RC6 == 0) { __delay_ms(100); while (PORTCbits.RC6==0); return '6'; }
           
            //Scan Keypad ROW3
             PORTCbits.RC0 = 1; PORTCbits.RC1 = 1; PORTCbits.RC2 = 0; PORTCbits.RC3 = 1;     
            if (PORTCbits.RC4 == 0) { __delay_ms(100); while (PORTCbits.RC4==0); return '4'; }
            if (PORTCbits.RC5 == 0) { __delay_ms(100); while (PORTCbits.RC5==0); return '8'; }
            if (PORTCbits.RC6 == 0) { __delay_ms(100); while (PORTCbits.RC6==0); return '9'; }
            
           //Scan Keypad ROW4
            PORTCbits.RC0 = 1; PORTCbits.RC1 = 1; PORTCbits.RC2 = 1; PORTCbits.RC3 = 0;    
            if (PORTCbits.RC4 == 0) { __delay_ms(100); while (PORTCbits.RC4==0); return '7'; }
            if (PORTCbits.RC5 == 0) { __delay_ms(100); while (PORTCbits.RC5==0); return '0'; }
            if (PORTCbits.RC6 == 0) { __delay_ms(100); while (PORTCbits.RC6==0); return '#'; }
                      
        return 'n';   
    }

void Keypad_Init(void)
{
    Keypad_PORT = 0xF0;   // Last 4 pins input, First 4 pins output 
   
  
}
void LCD_Init()
    {
        __delay_ms(15); /* 15ms,16x2 LCD Power on delay */
        LCD_Port = 0x00; /* set PORT as output PORT for LCD data(D0-D7) pins */
        LCD_Control = 0xFC; /* set PORT as output PORT LCD Control(RS,EN) Pins */
        LCD_Command(0x38); /* uses 2 line and initialize 5*8 matrix of LCD */
        LCD_Command(0x01); /* clear display screen */
        LCD_Command(0x0C); /* display on cursor off */
        LCD_Command(0x06); /* increment cursor (shift cursor to right) */ 
    } 
void LCD_Command(char cmd )
    {
        ldata = cmd; /* Send data to PORT as a command for
        LCD */
        RS = 0; /* Command Register is selected */
        EN = 1; /* High-to-Low pulse on Enable pin to latch
        data */
        EN = 0;
        __delay_ms(3);
    }
void LCD_Clear()
    {
        LCD_Command(0x01);  //clear LCD Display
        LCD_Command(0x80);  //Move cursor to beginning of 1st Lane 
    }
void LCD_Char(char dat)
    {
        ldata = dat; /* Send data to LCD */
        RS = 1; /* Data Register is selected */
        EN=1; /* High-to-Low pulse on Enable pin to latch data */
        EN = 0;  
        __delay_ms(1);   
    }
void LCD_String(const char *msg)
    {
        while((*msg)!=0)
        {
        LCD_Char(*msg);
        msg++;
        }   
    } 
void LCD_GoTo(char column,char line)
{
    // if incorrect line or column
    if ((line > (NUM_LINES-1)) || (column > (NUM_COL-1)))
    {
    // Just do nothing
    return;
    }
    // LCD_Goto command
    LCD_Command((line == 1) ? (0xC0 | column) : (0x80 | column));

    // Wait for the LCD to finish
    __delay_ms(5); //5ms delay
    }
void Ask_User()
    {
        
        LCD_Clear(); //Clear LCD
        LCD_String("Enter USER"); //Print Message to LCD
        LCD_GoTo(0,1); //Point Cursor to Line 2 Space 1
        User1 = GetKey(); //Declares variable from keypress
        LCD_Char(User1);  //Display keypress to LCD
        
        LCD_GoTo(0,2); //Point Cursor to Line 2 Space 2
        User2 = GetKey(); //Declares variable from keypress
        LCD_Char(User2); //Display keypress to LCD
        
        LCD_GoTo(0,3); //Point Cursor to Line 2 Space 3
        User3 = GetKey(); //Declares variable from keypress
        LCD_Char(User3); //Display keypress to LCD
        
        EnterUser = GetKey(); //Declares variable from keypress
        
        if(User1 == '0' && User2 == '3' && User3 == '2' && EnterUser == '#')
        {
            LCD_Clear(); //Clear LCD
            LCD_String("PASS"); //Print Message to LCD
            UserA = 1; //Set flag to HIGH indicating the User account used
            __delay_ms(2000);  //delay of 2 sec 
        }
        else if(User1 == '6' && User2 == '8' && User3 == '3' && EnterUser == '#')
        {
            LCD_Clear(); //Clear LCD
            LCD_String("PASS"); //Print Message to LCD
            UserB = 1; //Set flag to HIGH indicating the User account used
            __delay_ms(2000); //delay of 2 sec
        }
        else if(User1 == '2' && User2 == '3' && User3 == '2' && EnterUser == '#')
        {
            LCD_Clear(); //Clear LCD
            LCD_String("PASS"); //Print Message to LCD
            UserC = 1; //Set flag to HIGH indicating the User account used
            __delay_ms(2000); //delay of 2 sec
            
        }
       else{
            LCD_Clear(); //Clear LCD
            LCD_String("WRONG"); //Print Message to LCD
            __delay_ms(2000);  //delay of 2 sec
        }  
    }
void Ask_Password()
    {
        LCD_Clear(); //Clear LCD
        LCD_String("Enter Password"); //Print Message to LCD
        LCD_GoTo(0,1); //Point Cursor to Line 2 Space 1
        Pass1 = GetKey(); //Declares variable from keypress
        LCD_Char(Pass1); //Display keypress to LCD
        
        LCD_GoTo(0,2); //Point Cursor to Line 2 Space 2
        Pass2 = GetKey(); //Declares variable from keypress
        LCD_Char(Pass2); //Display keypress to LCD
        
        LCD_GoTo(0,3); //Point Cursor to Line 2 Space 3
        Pass3 = GetKey(); //Declares variable from keypress
        LCD_Char(Pass3); //Display keypress to LCD
        
        LCD_GoTo(0,4); //Point Cursor to Line 2 Space 4
        Pass4 = GetKey(); //Declares variable from keypress
        LCD_Char(Pass4); //Display keypress to LCD
        
        LCD_GoTo(0,5); //Point Cursor to Line 2 Space 5
        Pass5 = GetKey(); //Declares variable from keypress
        LCD_Char(Pass5); //Display keypress to LCD
        
        EnterPassword = GetKey(); //Declares variable from keypress
        
        if( UserA == 1 && Pass1 == '3' && Pass2 == '3' && Pass3 == '6' &&  Pass4 == '6' && Pass5 == '5'  && EnterPassword == '#')
        {
            LCD_Clear(); //Clear LCD
            LCD_String("PASS"); //Print Message to LCD
            __delay_ms(2000); //delay of 2 sec
            RA0 = 1; //Turn on LED
            __delay_ms(2000); //delay of 2 sec
            LCD_Clear(); //Clear LCD
            LCD_String("Entry Gate Open"); //Print Message to LCD
            __delay_ms(2000); //delay of 2 sec
            RA0 = 0; //Turn off LED
            vehicle++; //Increase vehicle counter by 1  
        }
        else if( UserB == 1 && Pass1 == '8' && Pass2 == '3' && Pass3 == '6' &&  Pass4 == '8' && Pass5 == '0'  && EnterPassword == '#')
        {
            LCD_Clear(); //Clear LCD
            LCD_String("PASS"); //Print Message to LCD
            __delay_ms(2000);
            RA0 = 1; //Turn on LED
            __delay_ms(2000);
            LCD_Clear(); //Clear LCD
            LCD_String("Entry Gate Open"); //Print Message to LCD
            __delay_ms(2000); //delay of 2 sec
            RA0 = 0; //Turn off LED
            vehicle++;  //Increase vehicle counter by 1  
        }
        else if( UserC == 1 && Pass1 == '2' && Pass2 == '2' && Pass3 == '8' &&  Pass4 == '8' && Pass5 == '9'  && EnterPassword == '#')
        {
            LCD_Clear(); //Clear LCD
            LCD_String("PASS"); //Print Message to LCD
            __delay_ms(2000); //delay of 2 sec
            RA0 = 1; //Turn on LED
            __delay_ms(2000); //delay of 2 sec
            LCD_Clear(); //Clear LCD
            LCD_String("Entry Gate Open"); //Print Message to LCD
            __delay_ms(2000); //delay of 2 sec
            RA0 = 0; //Turn off LED
            vehicle++;  //Increase vehicle counter by 1   
        }
        else
        {
            LCD_Clear(); //Clear LCD
            LCD_String("WRONG"); //Print Message to LCD
            __delay_ms(2000); //delay of 2 sec
        }
    }

/* end of parkinggarage.c */

