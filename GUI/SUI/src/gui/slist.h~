#ifndef SLIST_H
#define SLIST_H

#include "scontrol.h"
#include "stable.h"
#include "sscrolled.h"

struct __list_link
{
	wchar_t **str;
	int size;
	int beginpos;
	__list_link *next;
};

typedef struct  __list_link list_link;


class sList:public sControl
{
public:
	sList(int row, int line = 16);
	virtual ~sList();
	
	virtual void update();
	virtual void keyEvent(class sEvent *e);
	
	bool setTitle(char **str);
	bool setTitle(const char **str);
	bool insertLine(char **str, int y = -1);
	bool insertLine(const char **str, int y = -1);
	void adapt();
	
	int getCurrentLine(){ return m_table->getCurrentLine(); };
	int getCount(){ return m_table->getCountLine(); };
	
	wchar_t* getXyText(int x, int y);
	bool	setXyText(int x, int y, char *str);
	void prepare();
	
	int row(){ return m_table->row(); };
	int line(){ return m_table->line(); };
  
private:
	class sTable *m_table;
	class sScrolled *m_scrolled;
	
};


#endif
