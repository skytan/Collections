#include "stable.h"
#include "sencoded.h"
#include <wchar.h>

sTable::sTable(int row, int line, int textsize)
{
	m_enable_focus = true;
	
	m_start = 0;
	m_count = 0;
	m_current = 0;
	
	m_auto_update = true;
	
	m_max_x = row;
	m_max_y = line;
	m_textsize = textsize;
	
	m_title = 0;
	m_height = 16;
	
	setRect(0, 0, getXres(), getYres());
	
	
	m_table = new wchar_t[ m_max_x * m_max_y * m_textsize + 4 ]();
	if( !m_table ) perr("%s", "");
	bzero(m_table, m_max_x * m_max_y * m_textsize * sizeof(wchar_t) );
	
	m_row_right = new int[ m_max_x + 1 ]();
	if( !m_row_right ) perr("%s", "");
	
	int rowsize = (m_right - m_left) / m_max_x;
	for(int i = 0; i <= m_max_x; i++) m_row_right[i] = m_left + i * rowsize;
}


sTable::~sTable()
{
	delete m_table;
	delete m_row_right;
}

void sTable::keydown(class sEvent *e)
{
	if ( m_current >= m_count - 1 ) return;
	
	m_current++;
	if( m_current - m_start >= viewLine() ) m_start++;
	
	e->setType(0);
	if( m_auto_update ) update();
}

void sTable::keyup(class sEvent *e)
{
	if( m_current <= 0 ) return;
	
	m_current--;
	if( m_start > m_current ) m_start--;
	
	e->setType(0);
	if( m_auto_update ) update();
}

void sTable::keyright(class sEvent *e)
{

}

void sTable::keyleft(class sEvent *e)
{
}

void sTable::keyEvent(class sEvent *e)
{
	wchar_t c = e->getValue();
	
	switch( c )
	{
		case KEY_UP:
			return keyup(e);
		case KEY_DOWN:
			return keydown(e);
		case KEY_RIGHT:
			return keyright(e);
		case KEY_LEFT:
			return keyleft(e);
		default:
			break;
	}
}

void sTable::setLineHeight(int h)
{
	if( 0 >= h )
	{
		m_height = (m_buttom + 1 - m_top) / viewLine() ;
	}else
	{
		m_height = h;
	}
}

int sTable::lineHeight()
{
	return m_height;
}

int sTable::viewLine()
{
	return (m_buttom + 1 - m_top) / lineHeight();
}

void sTable::update()
{
	fillRect();
	drawTable();
	
	if( m_count > 0 )
	{
		for(int y = m_start; (y < m_count) && ((y - m_start) < viewLine()) ; y++)
			drawLine(y);
		
		reverseLine(m_current);
	}else
	{
		drawTitle();
	}	
}

void sTable::reverseLine(int y)
{
	int top = m_top + (m_current - m_start) * lineHeight();
	
	reverseRect(m_left + 1, top + 1, m_right - 1, top + lineHeight() - 1 );
}

void sTable::drawTitle()
{	
	for(int x = 0; (x < m_max_x) && (m_row_right[ x ] <= m_right); x++)
	{
		drawXy(x, 0, m_title + (x * m_textsize));
	}
}

void sTable::drawLine(int y)
{	
//	if( y == m_current ) fillRect(left + 3, top + (m_current - m_start) * lineHeight() + 2, right - 3, top + (m_current + 1 - m_start) * lineHeight() - 3, 1);
	
	for(int x = 0; (x < m_max_x) && (m_row_right[ x ] <= m_right); x++)
	{
		drawXy(x, y);
	}
}

void sTable::drawTable()
{
	int height = lineHeight();
	for(int i = 1; 1; i++)
	{
		if( m_top + i * height > m_buttom ) break;
		drawxLine(m_left, m_top + i * height, m_right - m_left);
	}
	
	for(int i = 1; i < m_max_x; i++)
	{
		drawyLine(m_row_right[i], m_top, m_buttom - m_top);
	}
	
	drawRect();
}

int sTable::drawXy(int x, int y, wchar_t *p)
{
//	if( m_row_right[x] > right || top + (y - m_start) * lineHeight() >= buttom ) return 0;
	wchar_t *str;
	int xpos = m_row_right[x] + 2;
	int ypos = m_top + (y - m_start) * lineHeight() + ( (lineHeight() - get_font_height()) >> 1 );

	if( p )
	{
		str = p;
	}else
	{
		str = xyText(x, y);
	}
	
	for(int i = 0; str[i]; i++)
	{
		if( xpos + get_font_weight( str[i] ) >= m_row_right[x+1] ) return xpos;
		
		if( 1 || y != m_current ) 
		{
			xpos += drawFont(xpos, ypos, str[i]);
		}else
		{
			xpos += drawFontReverse(xpos, ypos, str[i]);
		}
	}
	
	return xpos;
}

wchar_t* sTable::getXyText(int x, int y)
{
	return xyText(x, y);
}

wchar_t* sTable::xyText(int x, int y)
{
	return m_table + ( y * m_max_x + x ) * m_textsize;
}

bool sTable::setTitle(char **str)
{
	m_title = new wchar_t[ m_textsize * m_max_x ];
	if( !m_title )return false;
	
	for(int i = 0; i < m_max_x; i++)
		if( str[i] ) utf8_to_unicode(str[i], m_title + i * m_textsize, m_textsize -1);
	
	return true;
}

bool sTable::setXyText(int x, int y, char *str)
{
	if( x >= m_max_x || y >= m_count ) return false;
	
	utf8_to_unicode(str, xyText(x, y), m_textsize -1);
	
	update();
	return true;
}

bool sTable::insertLine(wchar_t **str, int y)
{
	if( 0 > y ) y = m_count;
	
	memmove(xyText(0, y + 1), xyText(0, y), (m_count - y) * m_max_x * m_textsize * sizeof(wchar_t) );
	
	wchar_t *p;
	for(int i = 0; i < m_max_x; i++)
	{
		if( !str[i] ) continue;
		
		p = xyText(i, y);
		wcsncpy(p, str[i], m_textsize -1);
		p[m_textsize -1] = L'\0';
	}
	
	m_count++;
	return true;
}

bool sTable::insertLine(char **str, int y)
{
	if( 0 > y ) y = m_count;
	
	memmove(xyText(0, y + 1), xyText(0, y), (m_count - y) * m_max_x * m_textsize * sizeof(wchar_t) );
	
	for(int i = 0; i < m_max_x; i++)
	{
		if( str[i] ) utf8_to_unicode(str[i], xyText(i, y), m_textsize -1);
	}
	
	m_count++;
	return true;
}


