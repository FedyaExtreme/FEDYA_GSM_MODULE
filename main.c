    //ATmega8

#include <intrinsics.h>
#include <iom8.h>
//#include <iom16.h>
#include <ina90.h>
#include "define.c"
#include <stdint.h> //для типов данныъ uint6_t


#define BUTTON_PIN 					PINB
//#define KN0_BIT 					PB0
//#define KN1_BIT 					PB1
//#define KN2_BIT 					PB2
//#define KN4_BIT 					PB4
//#define KN5_BIT					        PB5
//#define KN6_BIT                                         PB6

//#define KEY_KN0					        0x01
//#define KEY_KN1					        0x02
//#define KEY_KN2				                0x04
//#define KEY_KN4					        0x10
//#define KEY_KN5					        0x20
//#define KEY_KN6                                         0x40


//===========================================================================   
//=========================================================================
unsigned char knopka_press_S1=0;
unsigned char knopka_press_S2=0;
unsigned char knopka_press_S3=0;
unsigned char knopka_press_S4=0;
unsigned char knopka_press_S5=0;

unsigned char knopka_S4; 
unsigned char knopka_S3;
unsigned char knopka_S5;
unsigned char knopka_S2;
unsigned char knopka_S1;

unsigned char n1;
unsigned char n2;
unsigned char n3;
unsigned char n4;
unsigned char n5;
unsigned char cmd;

unsigned char  len;
//char cmd1[3]={'A','T','/n'}; // строка с АТ командой
unsigned char Send_buf[10] = {0}; 

volatile char buffer[BUF_SIZE]="";
volatile char inbuf[IN_BUF_SIZE]="$"; //inner buffer of USART
volatile uint8_t ind_in=0, ind_out=0, rxind_out=0, rxind_in=0, mess = 0;
volatile uint8_t com_detect=0;  //сюда будет записана обнаруженная команда

//============================================================================
void init_timer0()
{
  TIMSK = (1<<TOIE0);// разрешить прерывание по переполнению таймера 0
  TCCR0 = (1<<CS02)|(0<<CS01)|(0<<CS00); //clk/256
 
}

//===============================================================================  
void init_io()
{
  __disable_interrupt();
 // DDRD = 0x02; // Ustanovka porta D na RX i TX
  //PORTD = 0x00; // nuli
  DDRB = 0x00; // Ustanovka porta B na vhod
  PORTB = 0xFF; // подтягивающие резисторы
  
  }
 //============================================================================== 

void init_UART()
{
	UBRRH=0;	//	UBRR=f/(16*band)-1 f=8000000?? band=9600, 
	UBRRL=51;
        UCSRB = (1<<RXEN)|(1<<TXEN)|(0<<RXCIE)|(0<<TXCIE);
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

//sending RS232 with interupt
void SendByte(char byte)
        {
        buffer[ind_in++] = byte;
        ind_in &= BUF_MASK;
        }

void SendStr(char *string)
        {
        while (*string!='\r')  //check if End
                {
                SendByte(*string);
                string++;
                }
        }





//=============================================================================
void code_com (uint8_t count)
        {
        switch (com_detect)
                {
                case (0x12): if (count == 4) com_detect = 2; break; //R^I^N^G
                case (0x58): if (count == 5) com_detect = 3; break; //ERROR
                case (0x04): if (count == 2) com_detect = 1; break; //OK
                case (0x5C): if (count == 3) com_detect = 4; break; //ATI
                default: com_detect = 0;
                }
        }


//=================================================================================
//==============================================================================

unsigned char getch_Uart(void)//    Получение байта
          {
while(!(UCSRA&(1<<RXC))) //  Устанавливается, когда регистр свободен
          {}
return UDR;
}

//===============================================================================

//================================================================================

void rx_check_in (void)
    {
    uint8_t count=0;
    com_detect = 0;  //обнуление команды (чтобы не мешал предыдущий мусор)
    while (1)
        {
        if (inbuf[rxind_out] != '$') //обнаружен конец команды (разделитель)
                {
                com_detect ^=  inbuf[rxind_out++]; //делаем XOR полученным символам
                rxind_out &= IN_BUF_MASK;
                count++;  //считаем, сколько символов в команде
                }
        else 
                {
                rxind_out++;
                rxind_out &= IN_BUF_MASK;
                code_com (count);           //!! важная часть - раскодировать команду 
                break;
                }
        }
    }

//==================================================================================
//===============================================================================
void reaktsia_na_otvet() 
{    
   
     {
 if (mess != 0) //if we have mess in buffer
        {
        // code 
        mess--;   //minus one
      //  UCSRB &= ~(1 << RXCIE);//отключить прерывание приемника
        rx_check_in ();  //распознаём отдельную команду
        if (com_detect == 1)   //если была команда OK (код 1)
            {       
              SendStr("OK\r");
               SendByte(CR);    //отправляем <Enter> (0x0D)index = 0; //РџРµСЂРµРІРѕРґРёРј СѓРєР°Р·Р°С‚РµР»СЊ РЅР° РЅР°С‡Р°Р»Рѕ Р±СѓС„РµСЂР°

                UCSRB &= ~(1<<RXEN);   //Запрещаем прием на время отправки
                  UCSRB |= (1<<UDRIE);   //разрешаем прерывание по опустошению регистра данных UART                           //Посылаем сообщение
                                                // и принимаем входящие команды (OK)
         //   if (!send_sms (1,NUM0)) ErrMes (); //если после отправки не было команды OK
       //     }                                  //тогда выдать сообщение о ошибке протокола
        com_detect = 0; //обнуляем команду
    }
    else 
    {
      
     SendStr("OK\r");
               SendByte(CR);    //отправляем <Enter> (0x0D)
                UCSRB &= ~(1<<RXEN);   //Запрещаем прием на время отправки
                  UCSRB |= (1<<UDRIE);   //разрешаем прерывание по опустошению регистра данных UART  
      
    }
        }
     }
}
   


//=================================================================================

//==============================================================================

void proverka_klavish()
{ 
  
  if (knopka_S1 == 1)
  { 
    SendStr("AT+CMGF=1\r");
    SendByte(CR);    //отправляем <Enter> (0x0D)
    UCSRB &= ~(1<<RXEN);   //Запрещаем прием на время отправки
    UCSRB |= (1<<UDRIE);   //разрешаем прерывание по опустошению регистра данных UART 
    knopka_S1 = 0;
    TCNT0=0x00; //Обнуляем таймер счетчик 0
    TIFR=0x00; //Продолжаем ожидать нажатия
  }

if (knopka_S2 == 1)
{
  
  
    SendStr("AT\r");
    SendByte(CR);    //отправляем <Enter> (0x0D)
    UCSRB &= ~(1<<RXEN);   //Запрещаем прием на время отправки
    UCSRB |= (1<<UDRIE);   //разрешаем прерывание по опустошению регистра
    knopka_S2=0;
    TCNT0=0x00; //Обнуляем таймер счетчик 0
    TIFR=0x00; //Продолжаем ожидать нажатия
    
     
}
 
if (knopka_S3 == 1)
{//send_UART_str(2);
//send_UART(0x0d);
knopka_S3=0;}
 
if (knopka_S4 == 1)
{//send_UART_str(2);
//send_UART(0x0d);
knopka_S3=0;}
   
       

  __enable_interrupt();
}
//===================================================================================
//===================================================================================
 #pragma vector = TIMER0_OVF_vect  
__interrupt void knopka() 
{ 
         if(!PINB_Bit4 && knopka_press_S4) n4++;
	 if(PINB_Bit4){knopka_press_S4=1; n4=0;}

	 if(!PINB_Bit3 && knopka_press_S3) n3++; //Если кнопка НАЖАТА,а до того была НЕ НАЖАТА.
         if(PINB_Bit3){knopka_press_S3=1; n3=0;} //Если кнопка  НЕ НАЖАТА.
	 
	 if(!PINB_Bit5 && knopka_press_S5) n5++;
	 if(PINB_Bit5){knopka_press_S5=1; n5=0;}
	 
	 if(!PINB_Bit2 && knopka_press_S2) n2++;
	 if(PINB_Bit2) {knopka_press_S2=1; n2=0;}
	 
	 if(!PINB_Bit1 && knopka_press_S1) n1++;
	 if(PINB_Bit1){knopka_press_S1=1; n1=0;}
     
       //TCNT0=0x00; //Обнуляем таймер счетчик 0
       //TIFR=0x00; //Продолжаем ожидать нажатия
     __enable_interrupt();
     
} 
 

//Sending data from buffer
 #pragma vector = USART_UDRE_vect  
__interrupt void send()          
        {
        UDR = buffer[ind_out++];  //запись из буфера
        ind_out &= BUF_MASK;      //проверка маски кольцевого буфера
        if (ind_in == ind_out)  //если буфер уже пуст
                {
                 UCSRB &= ~(1<<UDRIE); //disable instrupt UDR empty
                  // UCSRB |=(1<<RXEN);     //разрешение приема поссле окончания отправки
                 UCSRB |= (1<<RXEN)|(1<<RXCIE);// активация прирывания по завершению приема
                TIMSK &= ~(1<<TOIE0);// запрещаем прерывание от таймера 0
                }
        __enable_interrupt();
        }

//recieving Data from RS232
 #pragma vector = USART_RXC_vect  
__interrupt void recieve()            
{
        uint8_t tmp;
        tmp = UDR;
        if (tmp == 0x0D)        //получен конец команды - <enter>
                {
                mess++;    //one more message
                inbuf[rxind_in++] = '$'; //вставляем разделитель в буфер
                rxind_in &= IN_BUF_MASK;
                }
    else 
                {
                if (tmp != 0x0A) //очистка непонятного символа с модуля
                        {
                        inbuf[rxind_in++] = tmp;   //записываем в буфер
                        rxind_in &= IN_BUF_MASK;
                        }
                }
         TIMSK |= (1<<TOIE0);// разрешаем прерывание от таймера 0
          
        __enable_interrupt ();
       
}




//===============================================================================
//===============================================================================

int main( void )
{ 
  __disable_interrupt();
  init_timer0();
  init_io();
  init_UART();
   
   
  __enable_interrupt();
   
   
    while(1)
    {
                
                if(n1>10 ){knopka_S1 = 1;} else {knopka_S1 = 0;} 
		if(n2>10 ){knopka_S2 = 1;} else {knopka_S2 = 0;} 		
		if(n3>10 ){knopka_S3 = 1;} else {knopka_S3 = 0;}
		if(n4>10 ){knopka_S4 = 1;} else {knopka_S4 = 0;} 	
		if(n5>10 ){knopka_S5 = 1;} else {knopka_S5 = 0;} 
	 
         if (knopka_S4)
         {     knopka_press_S4 = 0;
               n4 = 0;
         }
         if (knopka_S3)
         {     knopka_press_S3 = 0;
               n3 = 0;
         }
         if (knopka_S2)
         {     knopka_press_S2 = 0;
               n2 = 0;
         }
          if (knopka_S1)
         {     knopka_press_S1 = 0;
               n1 = 0;
         }
         
       //  if(UCSRA&(1<<RXC))
      //   {   
       //    SendByte(getch_Uart());
         //  SendByte(CR);    //отправляем <Enter> (0x0D)
         //  UCSRB &= ~(1<<RXEN);   //Запрещаем прием на время отправки
         //  UCSRB |= (1<<UDRIE);   //разрешаем прерывание по опустошению регистра
       //  }
    proverka_klavish();
    
    reaktsia_na_otvet();
   
   __enable_interrupt();
    
    }
}
