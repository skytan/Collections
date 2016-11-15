#ifndef SENTRY_H
#define SENTRY_H

#include "scontrol.h"

class sEntry:public sControl
{
public:
	sEntry(int maxlen = 0);
	
	virtual void update();
	virtual void timer();
	virtual void keyEvent(class sEvent *e);
	virtual void textExchange();
	virtual void focusExchange();

protected:
	void addWchar(wchar_t w);
	void delWchar();

	int drawTextLeft();
	int drawTextRight();
	int drawTextY();

	int cursorX();
	void cursorLeft();
	void cursorRight();

	bool cursorRightEd();

	void drawCursor(int colour = 1);
	
	
	void keydelete(class sEvent *e);

protected:
	int m_cursor;  //当前的光标位置
	int m_beginpos;  //第一个开始显示的字符位置
};

#endif
