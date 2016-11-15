#include "sentry.h"
#include "skeyevent.h"
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <stdio.h>

sEntry::sEntry(int mlen)
{
	m_isShow = true;
	m_isEdit = true;
	m_enable_edit = true;
	m_enable_focus = true;

	m_beginpos = 0;
	m_cursor = 0;
	
	max_text_len = mlen;
}

int sEntry::drawTextLeft()
{
	return m_left + 4;
}

int sEntry::drawTextRight()
{
	return m_right - 4;
}

int sEntry::drawTextY()
{
	return ((m_top + m_buttom + 1) >> 1) - (get_font_height() >> 1);
}

void sEntry::update()
{
 // perr();
 // wprintf(L"%s\n", text);
	fillRect();
	drawRect();
  
	int y = drawTextY();
	int x = drawTextLeft();
	int xmax = drawTextRight();
  
	for( int i = m_beginpos; m_text[i]; i++ ) 
	{
		if( x + get_font_weight( m_text[i] ) > xmax ) return;
		
		x += drawFont(x, y, m_text[i]);	     
	}

	drawCursor(m_selected);
}

int sEntry::cursorX()
{
	return drawTextLeft() + get_text_weight(m_text + m_beginpos, m_cursor - m_beginpos);
}

void sEntry::delWchar()
{
	if( !m_text[0] || m_cursor <= 0 ) return;

	int i = m_cursor;
	while( m_text[i-1] = m_text[i] ) i++;
	
	cursorLeft();
}

void sEntry::addWchar(wchar_t w)
{
	int i = m_cursor;
	int len = wcslen(m_text);
	print("%d", w);

	if( max_text_len > 0  )
	{
		if( len >= max_text_len ) return;
	}
	else
	{
		if(len >= count_of(m_text) - 1 ) return;
	}
	
//	for(int i = len + 1; i > cursor; i--) text[i] = text[i-1];
	memmove(m_text + m_cursor + 1, m_text + m_cursor, sizeof(m_text[0]) * (len + 1 - m_cursor) );
	
	m_text[m_cursor] = w;

	cursorRight();
}

void sEntry::textExchange()
{
/*
	cursor = 0;
	beginpos = 0;
	*/

	m_beginpos = 0;
	m_cursor = wcslen(m_text);

	if( cursorRightEd() )
	{
		m_beginpos = m_cursor - (drawTextRight() - drawTextLeft()) / get_font_weight();
	}

	m_beginpos = m_beginpos < 0 ? 0 : m_beginpos;

	this->update();
}

void sEntry::drawCursor(int colour)
{
	int x = cursorX();
	int y = drawTextY();	

	for(int i=0; i < get_font_height(); i++)
	{
		setPoint(x, y + i, colour);
	}
}

void sEntry::timer()
{
	static int time = 0;
	int colour;

	time++;
	colour = time;

	print("%s", "");
	drawCursor( colour & 1 );
}

bool sEntry::cursorRightEd()
{
	int textx = get_text_weight(m_text + m_beginpos, m_cursor - m_beginpos);

	return textx + drawTextLeft() > drawTextRight();
}

void sEntry::cursorRight()
{
	if( m_cursor >= wcslen(m_text) )
		return;
/*		
	if( cursor >= wcslen(text) ) 
	{
		beginpos = MIN(beginpos + 1, cursor - 1); 
		this->update();
		return;
	}
*/	
	m_cursor++;
	
	if( cursorRightEd() ) m_beginpos++;
	if( cursorRightEd() ) m_beginpos++;

	this->update();
	drawCursor(1);
}

void sEntry::cursorLeft()
{
	if( m_cursor > m_beginpos )
	{
		m_cursor--;
		if( m_cursor == m_beginpos && m_beginpos > 0 ) m_beginpos--;
	}
	else if( m_beginpos > 0 )
	{
		m_cursor = --m_beginpos;
	}
	
	this->update();
	drawCursor(1);
}

void sEntry::keydelete(class sEvent *e)
{
	m_text[0] = L'\0';
	m_cursor = 0;
	m_beginpos = 0;
	
	e->setType(0);
	update();
}

void sEntry::keyEvent(class sEvent *e)
{
	wchar_t w = e->getValue();
	print("%d", w);

	switch(w)
	{
		case KEY_RIGHT:
			cursorRight();
			e->setType(0);
			break;
		case KEY_LEFT:
			cursorLeft(); 
			e->setType(0);
			break;
		case KEY_BACKSPACE:
			delWchar();
			e->setType(0);
			break;
		case KEY_DELETE:
			return keydelete(e);
		default:
			break;
	}

	if( font_enable_draw(w) )
	{
		addWchar(w);
		e->setType(0);
	}
}

void sEntry::focusExchange()
{
	drawCursor(m_selected);
}
			
