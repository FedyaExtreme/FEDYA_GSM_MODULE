#define CR 0x0D

#define FOSC 8000000
#define BAUD 9600
#define MYUBRR (FOSC/(16*BAUD))-1

#define TIMEOUT 100     //на случай если команда так и не принята


#define BUF_SIZE 128
#define BUF_MASK (BUF_SIZE-1)
#define IN_BUF_SIZE 64
#define IN_BUF_MASK (IN_BUF_SIZE-1)
#define timer0_ON_interrupt   TIMSK_Bit7 = 1 
#define timer0_OFF_interrupt  TIMSK_Bit7 = 0  
#define SETBIT(ADDRESS, BIT)    (ADDRESS |= (1 << BIT))
#define CLEARBIT(ADDRESS, BIT)  (ADDRESS &= ~(1 << BIT))
#define CHECKBIT(ADDRESS, BIT)  (ADDRESS & (1 << BIT))
