#include <LPC214x.h>
#include "LCD7TDMI.h"
unsigned char ch[50];
char *str = "Welcome Prathamesh!!";

void uart_init()
{
	VPBDIV = 0x02;	//60/2 = 30MHz
	PINSEL0 |= (1<<18) | (1<<16);		//enable tx and rx bit
	U1LCR = 0x83;			//dlab on, no parity, 1 stop bit, 8 bit
	U1DLL = 0xB7;			//
	U1FDR = 0xF1;			//mul and div
	U1LCR = 0x03;			//disable dlab
	//U1IER = 0x01;			//enable interrupt
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

int main()
{
	uart_init();
	init_lcd();
	while(*str)
		{
			put_char(*str++);
		}
	while(1)
	{
		string_receive();
		string_trnsmit();
		send_cmd(0x01);// 0000 0001 Clear Display(): Clears display and returns cursor to the home position (address 0).
    send_cmd(0x02);// 0000 0010 Cursor Home(): 
		int j=0;
	  while(ch[j]!='\0')
	  {
		 send_data(ch[j]);
		 j++;
	  } 
		delay();
	}
}
