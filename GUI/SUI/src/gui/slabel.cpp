#include "slabel.h"

sLabel::sLabel(const char *str)
{
	if(str) setText(str);
}


sLabel::~sLabel()
{
}


void sLabel::update()
{
	int x = ((m_left + m_right) >> 1)  - (get_text_weight(m_text) >> 1);
	int y = ((m_top + m_buttom + 1) >> 1)  - (get_font_height() >> 1);
	
	for(int i = 0; m_text[i]; i++)
	{
		if( m_text[i] == L'\n' )
		{
			y += get_font_height() + 4;
			x = m_left;
		}
		else
		{
			x += drawFont(x, y, m_text[i]);
		}
	}
}
