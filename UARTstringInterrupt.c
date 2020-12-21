#include <LPC214x.h>
#include "LCD7TDMI.h"
unsigned char ch[50];
char str[] = "Welcome Prathamesh!!";

void uart_init()
{
	VPBDIV = 0x02;	//60/2 = 30MHz
	PINSEL0 |= (1<<18) | (1<<16);		//enable tx and rx bit
	U1LCR = 0x83;			//dlab on, no parity, 1 stop bit, 8 bit
	U1DLL = 0xB7;			//
	U1FDR = 0xF1;			//mul and div
	U1LCR = 0x03;			//disable dlab
	U1IER = 0x01;			//enable interrupt
}

int get_char(void)
{
		while(!(U1LSR & 0x01));
		return U1RBR;
}

int put_char(int ch)
{	
	while(!(U1LSR & 0x20));
	U1THR = ch;
	return U1THR;
}

void string_receive(void)
{
	int i=-1;
	do{
		i++;
		ch[i]=get_char();
	}while(ch[i]!='\r');
	ch[i]='\0';
}

void string_trnsmit(void)
{
	int j=0;
	while(ch[j]!='\0')
	{
		put_char(ch[j]);
		j++;
	}
}

void myisr() __irq
{	
	for(int i=0;str[i]!='\0';i++)
	 {
		 put_char(str[i]);
	 }
	
	send_cmd(0x01);// 0000 0001 Clear Display(): Clears display and returns cursor to the home position (address 0).
  send_cmd(0x02);// 0000 0010 Cursor Home(): 
	for(int j=0;str[j]!='\0';j++)
	 {
		 send_data(str[j]);
	 }
	delay();
	 
	VICVectAddr=0x00;
}

void uartinterrupt()
{
	VICIntEnable |= (1<<7);		//bit 7 enable
	VICVectCntl0 = 0x20|7;
	VICVectAddr0 = (unsigned int) myisr;
	U1IIR |= (1<<2);					//bit 2 recieve data available
}

int main()
{
	uart_init();
	init_lcd();
	uartinterrupt();
	while(1)
	{
	}
}
