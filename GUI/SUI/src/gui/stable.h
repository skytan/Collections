#ifndef S_TABLE
#define S_TABLE

#include "scontrol.h"

class sTable:public sControl
{
public:
	sTable(int row, int line = 16, int textsize = 32);
	~sTable();
	
	virtual void keyEvent(class sEvent *e);
	virtual void update();
	
	
	bool setTitle(char **str);
	bool insertLine(wchar_t **str, int y = -1);
	bool insertLine(char **str, int y = -1);
	bool setCurrentLine(int y);
	
	void setAutoUpdate(bool b) { m_auto_update = b; };
	
	wchar_t* getXyText(int x, int y);
	bool	setXyText(int x, int y, char *str);
	
	int getCountLine(){ return m_count; };
	int getStartLine(){ return m_start; };
	int getCurrentLine(){ return m_current; };
	
	void setLineHeight(int h = 0);
	int lineHeight();
	int viewLine();
	
	int row(){ return m_max_x; };
	int line(){ return m_max_y; };
	
protected:
	void drawLine(int y);
	void drawTitle();
	void reverseLine(int y);
	
	void keyup(class sEvent *e);
	void keydown(class sEvent *e);
	void keyleft(class sEvent *e);
	void keyright(class sEvent *e);
	
	void drawTable();
	int drawXy(int x, int y, wchar_t *p = 0);
	wchar_t* xyText(int x, int y);
	
private:
	wchar_t *m_title;
	wchar_t *m_table;
	int m_textsize;
	int m_max_x;
	int m_max_y;
	int m_count;
	
	int *m_row_right;
	
	int m_height;
	
	bool m_auto_update;
	
	int m_start;
	int m_current;
};

#endif
