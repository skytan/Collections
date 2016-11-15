#ifndef SCONTROL_H
#define SCONTROL_H

#include "sobject.h"
#include "swidget.h"


class sControl:public sObject
{
public:
	sControl();
	virtual ~sControl();   

	virtual void event(class sEvent *e){};
	virtual void keyEvent(class sEvent *e){};
	virtual void timer(){};
	virtual void click(class sEvent *e){};
    
	virtual void update(){};
	virtual void textExchange(){};
	virtual void focusExchange(){};

	int getTop(){return m_top;};
	int getLeft(){return m_left;};
	int getButtom(){return m_buttom;};
	int getRight(){return m_right;};
    
	int getMaxlen(){return max_text_len;};
	int setMaxlen(int len);
    
	void setRect(int top, int left, int buttom, int right);
    
	void drawRect(int colour = 1);
	void drawRect(int xleft, int ytop, int xright, int ybuttom, int colour = 1);
	void fillRect(int colour = 0);
	void fillRect(int left, int top, int right, int buttom, int colour = 0);
	
	void reverseRect(int xleft, int ytop, int xright, int ybuttom);
//    void drawRect(int top, int right, int buttom, int left);
	int drawFont(int x, int y, wchar_t word);
	int drawFontReverse(int x, int y, wchar_t word);
	int drawText(int x, int y, const wchar_t* text);
	int drawText(int x, int y, const char* text);
	
	void drawxLine(int x, int y, int size, int colour = 1);
	void drawyLine(int x, int y, int size, int colour = 1);

	char *getString();
	wchar_t *getText(){return m_text;};
	int setText(const wchar_t *str);
	int setText(const char *str);
	
	bool getSelected(){ return m_selected;};
	void setSelected(bool b) { m_selected = b;};
	
	bool getEnablefocus(){ return m_enable_focus; };
	void setEnablefocus(bool b) { m_enable_focus = b; };

	bool getEnableEdit(){ return m_enable_edit; };
	void setEnableEdit(bool b) { m_enable_edit = b; };

	bool getIsedit(){ return m_isEdit; };
    
protected:
	wchar_t m_text[128];
	int max_text_len;
	
	int m_top, m_left, m_buttom, m_right;
	
	bool m_selected;
	bool m_enable_focus;
	bool m_enable_edit;

	bool m_isShow;
	bool m_isEdit;

private:
};

#endif
