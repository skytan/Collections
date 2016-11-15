#ifndef SWIDGET_H
#define SWIDGET_H


#include "sobject.h"
#include "sevent.h"
#include "sframebuffer.h"
#include "skeyevent.h"
#include "sapplication.h"
#include "sfont.h"

struct shcut
{
	class sControl *c;
	wchar_t	key;
};

struct keyobject
{
	class sObject *object;
	const char	*name;
};

class sWidget;

typedef void (*s_callback)(class sObject *w, class sEvent *e, void *data);
#define S_CALLBACK(x) (s_callback)( (x) )

class sWidget:public sObject 
{
public:
	sWidget(int row = 0, int line = 0, int rowspace = 1, int linespace = 1);
	virtual ~sWidget();   

	virtual void event(class sEvent *e);
	virtual void keyEvent(class sEvent *e);
	virtual void timer(class sEvent *e){};
	virtual void click(class sEvent *e);
    
	virtual void update();
	virtual void textExchange(){};

	void areaUpdate(int t, int b);
    
	void show();  
	void destroy();
    
	class sControl* getCurrent(){return m_current;};
	bool setCurrent(int x, int y);
	bool setCurrent(class sControl *w);
	
	bool addControl(class sControl *w);
	bool addControl(class sControl *w, int x, int y);
	bool addControl(class sControl *w, int x, int y, int xpad, int ypad);

	class sControl* createLabel(int x, int y, const char *str, const char *name = 0);
	class sControl* createEntry(int x, int y, int textmaxlen = 0, const char *name = 0);
	class sControl* createButton(int x, int y, const char *str, s_callback fun = 0, void *data = 0, const char *name = 0);

	class sControl* createLabelEx(int x, int y, int xpad, int ypad, const char *str, const char *name = 0);
	class sControl* createEntryEx(int x, int y, int xpad, int ypad, int textmaxlen = 0, const char *name = 0);
	class sControl* createButtonEx(int x, int y, int xpad, int ypad, const char *str, s_callback fun = 0, void *data = 0, const char *name = 0);
	
	bool delControl(class sControl *w);

	bool getShow(){ return m_isShow;};
	void setShow(bool b){ m_isShow = b;};
    
	   
	class sControl* getIme(){return m_ime;};
	
	void registShortcut(class sControl *c, int key);
	void unregistShortcut(class sControl *c);
	void unregistShortcut(int key);
	
	s_callback getCallback(){ return m_callback; };
	void setCallback(s_callback fun, void *d = 0) { m_callback = fun; m_data = d;};

	bool addObject(const char *name, class sObject *object);
	bool delObject(class sObject *object);
	bool delObject(const char *name);
	class sObject* getObject(const char *name);
	class sControl* getControl(const char *name);

	char* getControlString(const char *name);
	wchar_t* getControlText(const char *name);
	void setControlText(const char *name, const char *text);
	void setControlText(const char *name, const wchar_t *text);
	bool setControlCurrent(const char *name);
	
private:
	void switchIme();
	void setImeRect();
	int getImeTop();
	
	void keyLeft();
	void keyRight();
	void keyUp();
	void keyDown();
	
	bool addControlXy(class sControl *w, int x, int y);
	   
protected:
	bool m_isShow;
	 
	s_callback m_callback;
	void *m_data;
  
private:
	class sControl* m_childs[8][8];
	int  m_childsCount;
	
	class sControl* m_current;
	int m_currentx, m_currenty;
  
	class sControl* m_ime;
	
	int m_rowsize, m_linesize;
	int m_rowspace, m_linespace;
	
	struct shcut m_shortcuts[ (1 + 'z') - '0'  ];

	struct keyobject m_object[16];
};

#endif
