#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void pdata(unsigned char *str)
{
	for(int i = 0; str[i]; i++)
	{
		if( (i + 1) % 8 == 0 )
		{
			printf("0x%x\n", str[i]);
		}else
		{
			printf("0x%x:", str[i]);
		}
	}

	printf("\n");
}


int main()
{
	unsigned char gbk[128] = {"»ú´òºÅÂë"};
	pdata(gbk);

	return 0;
}
