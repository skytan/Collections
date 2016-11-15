#ifndef SQAPPLICATION_H
#define SQAPPLICATION_H


#include "sevent.h"
#include "skeyevent.h"
#include "sperr.h"

#define MAX_STACK_COUNT 128

class sApplication
{
public:
	sApplication();
	~sApplication();	

	bool execWidget(class sWidget *w);
	
	bool getEventloop();
	void setEventloop(bool b);
	
protected:
	void sendEvent(class sEvent *e);
	void sendEvent(class sObject *w, class sEvent *e);   

	void beginEventloop();

	void sendUpdate();

	bool pushWidget(class sObject *widget);
	class sObject* popWidget();
     
	void sendMeaaage(class sEvent *e);
	void sendMeaaage(int type, int value = 0);
	
	void exit(int e = 0);

	void clearEvent();
private:
	class sObject* m_stack[MAX_STACK_COUNT];
	int m_stack_point;

	class sWidget *m_widget;
	
	class sEvent *m_event;
	
	class sEvent m_second_event;
	
	bool m_event_loop;
	
};


#endif
