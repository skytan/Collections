#ifndef SBUTTON_H
#define SBUTTON_H

#include "scontrol.h"

//typedef void (clickFun)();

typedef void (*click_callback)(class sWidget*, void*);
#define CLICK_CALLBACK(x) static_cast<s_callback>( (x) )

class sButton:public sControl
{
public:
	sButton(const char *text = 0, s_callback fun = 0, void *p = 0);
	virtual void update();
	virtual void focusExchange();
	virtual void click(class sEvent *e);
  
private:
	s_callback m_click_fun;
	void *m_click_data;
};

#endif
