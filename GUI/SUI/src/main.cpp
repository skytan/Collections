#include "window_main.h"
#include "mpm78m_printer.h"

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

int main(int argc, char* argv[])
{
	create_main_window();
}
