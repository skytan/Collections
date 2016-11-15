#include <stdio.h>
#include <stdarg.h>
#include "sperr.h"


void sp_data(const char *file, const char *fun, int line, const char *str, int len)
{	
	printf("%s::%s::%d\n\t", file, fun, line);
      
      for(int i=0; i<len; i++)
      {
            printf("%02x:", str[i]&0xff);
            if( !((i+1) & 0x3) ) printf("\n\t");
      }
      printf("\n");
}

int spretval(const char *file, const char *fun, int line, int ret)
{
	char buf[512];
	snprintf(buf, sizeof(buf), "%s::%s::%d\n", file, fun, line);
	
	perror(buf);
	
	return ret;
}