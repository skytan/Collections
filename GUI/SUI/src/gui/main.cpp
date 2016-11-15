#include <stdio.h>
/*
#include "sframebuffer.h"
#include "spinyin.h"
#include "font16.h"
*/
#include "swidget.h"
#include "scontrol.h"
#include "sperr.h"
#include "sbutton.h"
#include "sentry.h"
#include "slabel.h"
#include "sencoded.h"
#include "../common/mpm78m_printer.h"


void p_data(char *str)
{
	for(int i = 0; str[i]; i++)
	{
		if( (i + 1) % 8 == 0 )
		{
			printf("0x%x\n", 0xff & str[i]);
		}else
		{
			printf("0x%x:", 0xff & str[i]);
		}
	}
	
	printf("\n");
}

int main(int argc, char *argv[])
{
	char utf8[128] = {"机打号码"};
	char gbk[128];
	
	utf8_to_gbk2312( (const char *)utf8, gbk, sizeof(gbk));
	
//	p_data( utf8 );
	p_data( gbk );
	
	int i = 0;
/*	
	int printfd = open_printer_com();
	if( printfd < 0 ) return -1;
	
	mpm78m_print_utf8(printfd, (unsigned char*)utf8);
		
//	mpm78m_print_utf8(printfd, (unsigned char*)"机打号码:\n");
	
//	mpm78m_print_str(printfd, (unsigned char*)gbk);


	mpm78m_print_str(printfd, (unsigned char*)"\n\n\n\n");
	close_printer_com(printfd);

*/	
	return 0;
}