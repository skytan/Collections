#ifndef ST9IME_H
#define ST9IME_H


#include "spinyin.h"
#include "scontrol.h"
#include "sevent.h"

class sT9ime:public sControl
{
public:
	sT9ime(int top = 0, int left = 0);
	~sT9ime();

	void reset(int top = 0, int left = 0);

	virtual void keyEvent(class sEvent *e);
	virtual void update();
	
private:
	void alpha(class sEvent *e);
	void digit(class sEvent *e);
	void addsub(class sEvent *e);
	void backspace(class sEvent *e);
	
	void keyenter(class sEvent *e);
	void keyup(class sEvent *e);
	void keydown(class sEvent *e);
	void keyleft(class sEvent *e);
	void keyright(class sEvent *e);
	void keyzero(class sEvent *e);
	void keydit(class sEvent *e);
	
	int linefont();
	
	void keyUpdate(class sEvent *e = 0);

private:
	char m_pinyin[32];

	const struct t9_py *m_t9;
	
	int m_beginpos;
	int m_pos;
	int m_lineword;
	
//	static const int line_per_font = (s_fb.getXres() / get_font_weight() - 4) >> 1;
};
#endif
