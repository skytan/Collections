#include <wchar.h>
#include "scontrol.h"
#include "sencoded.h"
#include "sfont.h"
#include "sperr.h"

sControl::sControl()
{
	m_isEdit = false;
	m_isShow = false;
	m_selected = false;
	m_enable_edit = false;
	m_enable_focus = false;
	
	m_left = 0;
	m_top = 0;
	m_right = 0;
	m_buttom = 0;
	
	max_text_len = 0;
  
	bzero(m_text, sizeof(m_text)); 
}

sControl::~sControl()
{
}

int sControl::setMaxlen(int len)
{
	max_text_len = MIN(len, count_of(m_text) - 1);
	
	return max_text_len;
}


void sControl::setRect(int left, int top, int right, int buttom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_buttom = buttom;
}


void sControl::drawRect(int colour)
{	
	drawRect(m_left, m_top, m_right, m_buttom, colour);
}

void sControl::drawRect(int xleft, int ytop, int xright, int ybuttom, int colour)
{
//	printf("left:%dtop:%dright:%dbuttom:%d\n",  xleft, ytop, xright, ybuttom);
	
	getFramebuffer()->setXline(xleft, ytop, xright - xleft + 1, 1);
	getFramebuffer()->setXline(xleft, ybuttom, xright - xleft + 1, 1);
  
	drawyLine(xleft, ytop, ybuttom - ytop, colour);
	drawyLine(xright, ytop, ybuttom - ytop, colour);
}

void sControl::fillRect(int colour)
{
	for(int y = m_top; y <= m_buttom; y++)
	{
		getFramebuffer()->setXline(m_left, y, m_right - m_left + 1, colour);
	}
}

void sControl::fillRect(int l, int t, int r, int b, int colour)
{
	for(int y = t; y <= b; y++)
	{
		getFramebuffer()->setXline(m_left, y, m_right - m_left + 1, colour);
	}	
}

void sControl::reverseRect(int l, int t, int r, int b)
{
	for(int i = l; i <= r; i++) 
	{
		for(int j = t; j <= b; j++)
		{
			reversePoint(i, j);
		}
	}	
}

void sControl::drawxLine(int x, int y, int size, int colour)
{
	getFramebuffer()->setXline(x, y, size + 1, colour);	
}

void sControl::drawyLine(int x, int y, int size, int colour)
{
	for(int i = y; i <= y + size; i++) 
	{
		setPoint(x, i, colour);
	}	
}

int sControl::drawFont(int x, int y, wchar_t word)
{
	return draw_font(x, y, word, getFramebuffer());
}

int sControl::drawFontReverse(int x, int y, wchar_t word)
{
	return draw_font_reverse(x, y, word, getFramebuffer());
}
	

int sControl::drawText(int x, int y, const wchar_t* str)
{
	for(int i = 0; str[i]; i++)
	{		
		x += drawFont(x, y, str[i] );
	}

	return x;
}

int sControl::drawText(int x, int y, const char* str)
{
	wchar_t buf[128];

	utf8_to_unicode(str, buf, sizeof(buf));

	return drawText(x, y, buf);
}

char* sControl::getString()
{
	static char buf[ count_of(m_text) * 3 ];

	unicode_to_utf8(m_text, buf, sizeof(buf));

	return buf;
}

int sControl::setText(const wchar_t *str)
{
	if( !str ) return -1;
	
	if( max_text_len > 0 )
	{
		wcsncpy(m_text, str, max_text_len);		
	}
	else
	{  
		wcsncpy(m_text, str, count_of(m_text) - 1 );
	}
	
	this->textExchange();
	return wcslen(m_text);
}

int sControl::setText(const char *str)
{
	int len; 
	
	if( !str ) return -1;
  
	if( max_text_len > 0 )
	{
		len = utf8_to_unicode(str, m_text, max_text_len);
	}
	else
	{
		len = utf8_to_unicode(str, m_text, count_of(m_text) - 1);
	}
  
//  pdata(text, strlen((const char*)text));
	this->textExchange();
	return len;
}

  
  
  
  
