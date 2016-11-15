#ifndef SPYIME_H
#define SPYIME_H

#include "scontrol.h"
#include "sevent.h"

class sPyime:public sControl
{
public:
	sPyime(int top = 0, int left = 0);
	~sPyime();

	void reset(int top = 0, int left = 0);

	virtual void keyEvent(class sEvent *e);
	virtual void update();
	
protected:
	void alpha(class sEvent *e);
	void digit(class sEvent *e);
	void addsub(class sEvent *e);
	void backspace(class sEvent *e);
	
	int	linefont();

protected:
	char m_pinyin[32];
	wchar_t m_unicode[16];
	
	int m_beginpos;
	int m_pos;
	
	int m_lineword;
	
//	static const int line_per_font = (s_fb.getXres() / get_font_weight() - 4) >> 1;
};
#endif
