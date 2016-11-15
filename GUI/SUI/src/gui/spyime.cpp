#include <ctype.h>
#include <wchar.h>
#include <string.h>
#include "spyime.h"
#include "spinyin.h"
#include "sfont.h"
#include "swordgb2312.h"
#include "sperr.h"

//static const int sPyime::line_per_font = (getXres() / get_font_weight() - 4) >> 1;

sPyime::sPyime(int t, int l)
{
	m_top = t;
	m_buttom = m_top + get_font_height() + 4;
	
	m_left = l;
	m_right = getXres() - 1;
	
	m_beginpos = 0;
	m_lineword = (m_right - m_left) / get_font_weight();
	m_lineword = m_lineword > 32 ? 32 : m_lineword;
	
	print("%d", m_lineword);
	
	memset(m_pinyin, 0, sizeof(m_pinyin));
}

sPyime::~sPyime()
{
}

void sPyime::reset(int t, int l)
{
	m_top = t;
	m_buttom = m_top + get_font_height() + 4;
	
	m_left = l;
	m_right = getXres() - 1;
	
	m_pos = 0;
	m_beginpos = 0;
	m_pinyin[0] = '\0';
	m_text[0] = L'\0';

	update();
}


int sPyime::linefont()
{
	int i = ((m_lineword - 4) >> 1) - 1;
	
	return i > 10 ? 10 : i;
}
	
void sPyime::alpha(class sEvent *e)
{
	int len = strlen(m_pinyin);
	char c = static_cast<char>( e->getValue() );
	
	m_pinyin[len] = c;
	m_pinyin[len + 1] = '\0';
	
	const char *py = py_ime(m_pinyin);
	
	if(py)
	{
		setText(py);
	}
	else
	{
		m_text[0] = L'\0';
	}
	
	m_beginpos = 0;
	e->setType(0);
	this->update();
}

void sPyime::digit(class sEvent *e)
{
	int i = (e->getValue() - '0') + m_beginpos;
	
	if( wcslen( m_text ) > i )
	{
		e->setValue( m_text[i] );
		setText( get_gb2312_word( m_text[i] ) );
		
		m_beginpos = 0;
		m_pinyin[0] = '\0';
		this->update();
		return;
	}		
	
	e->setType( 0 );
}

void sPyime::backspace(class sEvent *e)
{
	int len = strlen(m_pinyin);     
	e->setType(0);
	
	if( len <= 0 )
	{
		return;
	}

	m_pinyin[ len - 1 ] = '\0';
	const char *py = py_ime(m_pinyin);
	
	if(py)
	{
		setText(py);
	}
	else
	{
		m_text[0] = L'\0';
	}

	this->update();
}

void sPyime::addsub(class sEvent *e)
{
	char c = static_cast<char>( e->getValue() );
	int i = linefont();
	
	if( c == '+' || c == '=' )
	{
		if( wcslen( m_text ) > m_beginpos + i ) m_beginpos += i;
	}
	else
	{
		if( m_beginpos >= i ) m_beginpos -= i;
	}
	
	e->setType(0);
	this->update();
}

void sPyime::keyEvent(class sEvent *e)
{
	unsigned char c = (unsigned char)(e->getValue());

	if( isalpha(c) )
	{
		return alpha(e);
	}
	else if( isdigit(c) )
	{
		return digit(e);
	}
	else if( c == KEY_BACKSPACE )
	{
		return backspace(e);
	}
	else if( c == '-' || c == '_' || c == '=' || c == '+')
	{
		return addsub(e);
	}
}


void sPyime::update()
{	
	int x = m_left + 2;
	int y = m_top + 4;
	wchar_t i;
	
	fillRect();
	drawRect();
	
	for(i = 0; m_pinyin[i]; i++)
	{
		x += drawFont(x, y, static_cast<wchar_t>( m_pinyin[i] & 0xff ));
	}
	
	x += drawFont(x, y, L':');
	
	int imax = m_beginpos + linefont();

	for(i = m_beginpos; i <  imax && m_text[i]; i++)
	{
		x += drawFont(x, y, (i - m_beginpos) + L'0' );
		x += drawFont(x, y, L'.' );
		x += drawFont(x, y, m_text[i] );
	}

	if( i == imax ) drawFont(x, y, L'>' );
}

