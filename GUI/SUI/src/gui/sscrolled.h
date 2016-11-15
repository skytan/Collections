#ifndef SSCROLLED_H
#define SSCROLLED_H

#include "scontrol.h"


class sScrolled:public sControl
{
public:
	sScrolled(int xleft = 0, int ytop = 0, int size = 0, bool verti = true);
	~sScrolled();
	
	virtual void update();
	
	void setBorder(int xleft, int ytop, int size);
	void setCountLine(int n){ m_count = n; };
	void setCurrentLine(int n){ m_current = n; };
	void setShowLine(int n){  m_show = n; };
	
	int wide(){ return 6; };
	
protected:
	void verticalUpdate();
protected:
	int m_count;
	int m_show;
	int m_current;
	bool m_vertical;
};


#endif