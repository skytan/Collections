#include "common.h"
#include "sui.h"

unsigned hash(const char * s)
{
	unsigned char ch;
	unsigned h = 4637;
	int len = strlen(s);
	
	while(len > 0)
	{
		ch = *s++ - 'A';
		
		if (ch <= 'Z' - 'A')
		{
			ch += 'a' - 'A';
		}
		
		h = ((h << 5) + h) ^ ch;
		--len;
	}
	
	return h;
}


int pretval(const char *file, const char *fun, int line, int ret)
{
	char buf[512];
	snprintf(buf, sizeof(buf), "%s::%s::%d\n", file, fun, line);
	
	perror(buf);
	
	return ret;
}

int wcstoi(const wchar_t *str)
{
	char buf[64];

	unicode_to_utf8(str, buf, sizeof(buf));

	return atoi(buf);
}
