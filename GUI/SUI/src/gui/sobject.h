#ifndef SOBJECT_H
#define SOBJECT_H

#include "sframebuffer.h"
#include "sapplication.h"

class sObject
{
public:
	sObject();
	~sObject(){};
	
	virtual void event(class sEvent *e){};
	virtual void keyEvent(class sEvent *e){};
	virtual void timer(class sEvent *e){};
	virtual void click(class sEvent *e){};
    
	virtual void update(){};
	virtual void textExchange(){};

	class sObject* getParent(){return parent;};
	void setParent(class sObject *w){ parent = w;};
	
	class sApplication* getApplication() { return &s_app; };
	class sFramebuffer* getFramebuffer() { return &s_fb; };
	
	int getXres();
	int getYres();
	void setPoint(int x, int y, int val);
	void reversePoint(int x, int y);
	void clearScreen() { s_fb.clearScreen();};	
	
private:
	static class sApplication s_app;
	static class sFramebuffer s_fb;
	
	class sObject* parent;
};

#endif
