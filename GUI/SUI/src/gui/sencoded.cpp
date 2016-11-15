#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>

#include "sperr.h"
#include "sgb2312.h"

int unicode_to_utf8_len(const wchar_t *in)
{
	int i = 0;
	wchar_t unicode;
	int outsize = 0;

	for(i = 0; unicode = in[i]; i++)
	{
		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			outsize += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			outsize += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			outsize += 3;
		}
	}

	return outsize;
}


int unicode_to_utf8(const wchar_t *in, char *out)
{
	int i = 0;
	wchar_t unicode;
	int outsize = 0;
	char *tmp = out;

	if( !out ) return unicode_to_utf8_len(in);

	for (i = 0; unicode = in[i]; i++)
	{
		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			*tmp++ = (char)unicode;
			outsize += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			*tmp++ = 0xc0 | (unicode >> 6);
			*tmp++ = 0x80 | (unicode & 0x3f);
			outsize += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			*tmp++ = 0xe0 | (unicode >> 12);
			*tmp++ = 0x80 | (unicode >> 6 & 0x3f);
			*tmp++ = 0x80 | (unicode & 0x3f);
			outsize += 3;
		}
	}

	*tmp = '\0';
	return outsize;
}

int unicode_to_utf8(const wchar_t *in, char *out, int len)
{
	int i = 0;
	wchar_t unicode;
	int outsize = 0;
	char *tmp = out;

	if( !out ) return unicode_to_utf8_len(in);

	for (i = 0; (unicode = in[i]) && i < len; i++)
	{
		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			if( outsize >= len -1 ) break;
			*tmp++ = (char)unicode;
			outsize += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			if( outsize >= len -2 ) break;
			*tmp++ = 0xc0 | (unicode >> 6);
			*tmp++ = 0x80 | (unicode & 0x3f);
			outsize += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			if( outsize >= len -3 ) break;
			*tmp++ = 0xe0 | (unicode >> 12);
			*tmp++ = 0x80 | (unicode >> 6 & 0x3f);
			*tmp++ = 0x80 | (unicode & 0x3f);
			outsize += 3;
		}
	}

	*tmp = '\0';
	return outsize;
}

int utf8_to_unicode_len(const char *in)
{
	const char *p = in;
	int resultsize = 0;

	while(*p)
	{
		if ( (*p & 0x80) == 0 )
		{
			p++;
		}
		else if ((*p & 0xe0)== 0xc0)
		{		  
			p += 2;
		}
		else if ((*p & 0xf0)== 0xe0)
		{
			p += 3;
		}
		
		resultsize ++;
	}

	return resultsize;
}

int utf8_to_unicode(const char *in, wchar_t *out)
{
	const char *p = in;
	wchar_t tmp;
	int i = 0;

	if( !out ) return utf8_to_unicode_len(in);

	while( *p )
	{
		if ( (*p & 0x80) == 0 )
		{
			tmp = *p++;
		}
		else if ((*p & 0xe0)== 0xc0)
		{		  
			tmp = (*p++ & 0x3f) << 6;
			tmp |= *p++ & 0x3f;
		}
		else if ((*p & 0xf0)== 0xe0)
		{
			tmp = (*p++ & 0x1f) << 12;
			tmp |= (*p++ & 0x3f) << 6;
			tmp |= *p++ & 0x3f;
		}
		
		out[i++] = tmp;
	}

	out[i] = 0;
	return i;
}

wint_t utf8_to_unicode(const char *in, char **out)
{
	wchar_t tmp;

	if ( (*in & 0x80) == 0 )
	{
		tmp = *in++;
	}
	else if ((*in & 0xe0)== 0xc0)
	{		  
		tmp = (*in++ & 0x3f) << 6;
		tmp |= *in++ & 0x3f;
	}
	else if ((*in & 0xf0)== 0xe0)
	{
		tmp = (*in++ & 0x1f) << 12;
		tmp |= (*in++ & 0x3f) << 6;
		tmp |= *in++ & 0x3f;
	}

	if( out ) *out = (char*)in;
	return tmp;
}

int utf8_to_unicode(const char *in, wchar_t *out, int len)
{
	const char *p = in;
	wchar_t tmp;
	int i = 0;

	if( !out ) return utf8_to_unicode_len(in);

	while( *p && i < len )
	{
		if ( (*p & 0x80) == 0 )
		{
			tmp = *p++;
		}
		else if ((*p & 0xe0)== 0xc0)
		{		  
			tmp = (*p++ & 0x3f) << 6;
			tmp |= *p++ & 0x3f;
		}
		else if ((*p & 0xf0)== 0xe0)
		{
			tmp = (*p++ & 0x1f) << 12;
			tmp |= (*p++ & 0x3f) << 6;
			tmp |= *p++ & 0x3f;
		}
		
		out[i++] = tmp;
	}

	out[i] = 0;
	return i;
}


int unicode_to_gbk2312(const wchar_t *in, char *out)
{
	char *tmp = out;
	while( *in )
	{
		if( *in < 0x80 )
		{
			*out++ = (char)*in++;
		}
		else
		{
			*(unsigned short*)out = unicode_to_gbk( *in++ );
			out += 2;
		}
	}
	
	*out++ = 0;
	return out - tmp;
}

int unicode_to_gbk2312(const wchar_t *in, char *out, int outsize)
{
	char *tmp = out;
	while( *in && ((out - tmp) < (outsize - 1)) )
	{
		if( *in < 0x80 )
		{
			*out++ = (char)*in++;
		}
		else
		{
			*(unsigned short*)out = unicode_to_gbk( *in++ );
			out += 2;
		}
	}
	
	*out++ = 0;
	return out - tmp;
}

int utf8_to_gbk2312(const char *in, char *out)
{
	char *tmp = out;
	while( *in )
	{		
		if( (unsigned char)*in < 0x80 )
		{
			*out++ = *in++;
		}else
		{
			unsigned gbk =  unicode_to_gbk( utf8_to_unicode(in, (char**)&in) );
			
			*(unsigned char*)(out + 1) = gbk;
			*(unsigned char*)out = gbk >> 8;
			out += 2;
		}
	}
	
	*out++ = 0;
	return out - tmp;
}

int utf8_to_gbk2312(const char *in, char *out, int outsize)
{
	char *tmp = out;
	while( *in && ((out - tmp) < (outsize - 1)) )
	{		
		if( (unsigned char)*in < 0x80 )
		{
			*out++ = *in++;
		}else
		{
			unsigned gbk =  unicode_to_gbk( utf8_to_unicode(in, (char**)&in) );
			*(unsigned short*)out = gbk;
			
			*(unsigned char*)(out + 1) = gbk;
			*(unsigned char*)out = gbk >> 8;
			out += 2;
		}
	}
	
	*out++ = 0;
	return out - tmp;
}


void dump_utf8(char *utf8)
{
	char c;
	while(c = *(utf8++))
	{
		printf("%02X:", c&0xff);
	}

	putchar('\n');
}

void dump_unicode(wchar_t *str)
{
	wchar_t w;
	while(w = *(str++))
	{
		printf("0x%04X\t", w&0xffff);
	}

	putchar('\n');
}
/*
int main()
{
	char utf[] = "你好你好你好";
	wchar_t uni[8];// = {0};

	utf8_to_unicode(utf, uni, sizeof(uni));
	dump_utf8(utf);
	dump_unicode(uni);

}
*/
