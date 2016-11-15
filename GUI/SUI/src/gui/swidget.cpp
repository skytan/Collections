#include <wctype.h>
#include <ctype.h>
#include <string.h>

#include "swidget.h"
#include "scontrol.h"
#include "sapplication.h"
#include "wchar.h"
#include "sencoded.h"
#include "spyime.h"
#include "slabel.h"
#include "sentry.h"
#include "sbutton.h"
#include "st9ime.h"


sWidget::sWidget(int r, int l, int rspace, int lspace)
{
	m_isShow = false;
  
	m_callback = 0;
	m_data = 0;
	m_current = 0;
	m_childsCount = 0;
  
	m_ime = 0;
	
	m_rowsize = r > 0 ? (getXres() / r) : getXres();
	m_linesize = l > 0 ? (getYres() / l) : getYres();
	
	m_rowspace = rspace;
	m_linespace = lspace;

	m_currentx = 0;
	m_currenty = 0;

	bzero(m_object, sizeof(m_object));
	bzero(m_shortcuts, sizeof(m_shortcuts));
	bzero(m_childs, sizeof(m_childs));  
}

sWidget::~sWidget()
{
	for(int i = 0; i < count_of( m_childs ); i++)
	{
		for(int j = 0; j < count_of( m_childs[0] ); j++)
		{
			if( m_childs[i][j] )
				delete m_childs[i][j];

		}
	}
	
	if( m_ime ) delete m_ime;

}

void sWidget::setImeRect()
{
	if( !m_ime ) return;
	
	int cb, ct;
	if( m_current ) 
	{
		cb = m_current->getButtom();
		ct = m_current->getTop();
	}
	else
	{
		cb = 0;
		ct = getYres() - 1;
	}
	
	if( 40 + cb > getYres() )
	{
		m_ime->setRect(0, ct - 28, getXres() - 1, ct - 4);
	}
	else
	{
		m_ime->setRect(0, cb + 4, getXres() - 1, cb + 28);
	}
	
	m_ime->update();
}

int sWidget::getImeTop()
{
	int ctop = m_current->getTop();
	int cbuttom = m_current->getButtom(); 
	int h = get_font_height() + 4;
	
	if( ctop > h )
	{
		return ctop - (h + 1);
	}
	else if( cbuttom + h < getYres() )
	{
		return cbuttom + 1;
	}
	else
	{
		return 0;
	}
}

void sWidget::switchIme()
{
	if( m_ime )
	{
		delete m_ime;
		m_ime = 0;
		
		this->update();
	}
	else
	{
		m_ime = new sT9ime( getImeTop() ); 
		m_ime->update();
	}
}

void sWidget::keyDown()
{
	int x = m_currentx;
	int y = m_currenty;
	
	do
	{
		
		if( y == count_of( m_childs[0] ) -1 && x == count_of( m_childs ) -1 )
		{
			y = 0;
			x = 0;
		}
		else if( y == count_of( m_childs[0] ) -1 )
		{
			y = 0;
			x++;
		}
		else
		{
			y++;
		}

		if( m_childs[x][y] && m_childs[x][y]->getEnablefocus() )
		{
			setCurrent(x, y );
			return;
		}

	}while( !(x == m_currentx && y == m_currenty) );
			
}


void sWidget::keyUp()
{
	int x = m_currentx;
	int y = m_currenty;
	
	do
	{
		
		if( y == 0 && x == count_of( m_childs[0] ) - 1 )
		{
			y = count_of( m_childs[0] ) - 1;
			x = 0;
		}
		else if( y == 0 )
		{
			y = count_of( m_childs[0] ) - 1;
			x++;
		}
		else
		{
			y--;
		}


		if( m_childs[x][y] && m_childs[x][y]->getEnablefocus() )
		{
			setCurrent(x, y );
			return;
		}
	}while( !(x == m_currentx && y == m_currenty) );
}

void sWidget::keyRight()
{
	int x = m_currentx;
	int y = m_currenty;
	
	do
	{
		
		if( x == count_of( m_childs ) - 1 &&  y == count_of( m_childs[0] ) -1 )
		{
			y = 0;
			x = 0;
		}
		else if( x == count_of( m_childs ) -1 )
		{
			x = 0;
			y++;
		}
		else
		{
			x++;
		}


		if( m_childs[x][y] && m_childs[x][y]->getEnablefocus() )
		{
			setCurrent(x, y );
			return;
		}
	}while( !(x == m_currentx && y == m_currenty) );
}

void sWidget::keyLeft()
{
	int x = m_currentx;
	int y = m_currenty;
	
	do
	{
		
		if( x == 0  &&  y == count_of( m_childs[0] ) -1 )
		{
			y = 0;
			x = count_of( m_childs ) - 1;
		}
		else if( x == 0 )
		{
			x = count_of( m_childs ) - 1;
			y++;
		}
		else
		{
			x--;
		}


		if( m_childs[x][y] && m_childs[x][y]->getEnablefocus() )
		{
			setCurrent(x, y );
			return;
		}
	}while( !(x == m_currentx && y == m_currenty) );
}

void sWidget::event(class sEvent *e)
{
	if( e->getType() == EVENT_TYPE_KEY &&  m_callback ) m_callback(this, e, m_data);
}

void sWidget::keyEvent(class sEvent *e)
{
	if( !e->getType() ) return;
	
	int c = e->getValue();
	
	if( isalnum( c ) )
	{
		if( m_shortcuts[ c - '0' ].c )
		{			
			(m_shortcuts[ c - '0' ].c)->click(e);
			e->setType(0);
			return;
		}
	}
	
	switch( c )
	{
		case KEY_TAB:
			switchIme();
			break;
		case KEY_UP:
			keyUp();
			break;
		case KEY_DOWN:
			keyDown();
			break;
		case KEY_LEFT:
			keyLeft();
			break;
		case KEY_RIGHT:
			keyRight();
			break;
		case KEY_QUIT:
			destroy();
			break;
		default:
			break;
	}

}

void sWidget::registShortcut(class sControl *c, int key)
{
	if( key < '0' || key > 'z' ) return;
	if( !c->getEnablefocus() ) return;
	
	m_shortcuts[ key - '0' ].c = c;
}

void sWidget::unregistShortcut(class sControl *c)
{
	for(int i = 0; i < count_of( m_shortcuts ); i++)
	{
		if( m_shortcuts[ i ].c == c )
			m_shortcuts[ i ].c = 0;
	}

}

void sWidget::unregistShortcut(int key)
{
	if( key < '0' || key > 'z' ) return;
	
	m_shortcuts[ key - '0' ].c = 0;
}

void sWidget::click(class sEvent *e)
{
}

void sWidget::update()
{
	if( !m_isShow ) return;
  
	clearScreen();
  
	for(int i = 0; i < count_of( m_childs ); i++)
	{
		for(int j = 0; j < count_of( m_childs[0] ); j++)
		{
			if(m_childs[i][j]) m_childs[i][j]->update();
		}
	}
  
	if(m_ime) m_ime->update();
}

void sWidget::areaUpdate(int t, int b)
{
	for(int i = t / m_linesize; i <= (b + m_linesize) / m_linesize; i++)
	{
		for(int j = 0; j < count_of( m_childs[0] ); j++)
		{
			if(m_childs[i][j]) m_childs[i][j]->update();
		}
	}
}

void sWidget::show()
{  
	m_isShow = true;
	getApplication()->execWidget(this);

	
	delete this;
}

bool sWidget::addControlXy(class sControl *w, int x, int y)
{
	wchar_t *str = w->getText();
	
	m_childs[x][y] = w;
	w->setParent(this);
	
	if( !m_current && w->getEnablefocus() ) setCurrent(x, y);
	if( iswalnum( str[0] ) ) registShortcut(w, str[0]);
	
	return true;
}

bool sWidget::addControl(class sControl *w)
{	
	for(int i = 0; i < count_of( m_childs ); i++)
	{
		for(int j = 0; j < count_of( m_childs[0] ); j++)
		{
			if( m_childs[i][j] == 0 )
				return addControlXy(w, i, j);
		}
	}
  
	return false;
}

bool sWidget::addControl(class sControl *w, int x, int y )
{	
	w->setRect(x * m_rowsize + m_rowspace, y * m_linesize + m_linespace, ((x + 1) * m_rowsize - 1) - m_rowspace, ((y + 1) * m_linesize - 1) - m_linespace);
	
	return  addControlXy(w, x, y);
}

bool sWidget::addControl(class sControl *w, int x, int y, int xpad, int ypad)
{	
	w->setRect(x * m_rowsize + m_rowspace, y * m_linesize + m_linespace, ((x + xpad) * m_rowsize - 1) - m_rowspace, ((y + ypad) * m_linesize - 1) - m_linespace);
	
	return  addControlXy(w, x, y);
}


class sControl* sWidget::createLabel(int x, int y, const char *str, const char *name)
{
	class sControl *c = new sLabel(str);

	if( !c ) return 0;

	if( addControl(c, x, y) &&  addObject(name, c)  )
	{
		return c;
	}
	else
	{
		delete c;
	}

	return 0;
}

class sControl* sWidget::createLabelEx(int x, int y, int xpad, int ypad, const char *str, const char *name)
{
	class sControl *c = new sLabel(str);

	if( !c ) return 0;

	if( addControl(c, x, y, xpad, ypad) &&  addObject(name, c) )
	{
		return c;
	}
	else
	{
		delete c;
	}

	return 0;
}


class sControl* sWidget::createEntry(int x, int y,  int textmaxlen, const char *name)
{
	class sControl *c = new sEntry(textmaxlen);

	if( !c ) return 0;

	if( addControl(c, x, y) &&  addObject(name, c)  )
	{
		return c;
	}
	else
	{
		delete c;
	}

	return 0;
}

class sControl* sWidget::createEntryEx(int x, int y, int xpad, int ypad, int textmaxlen, const char *name)
{
	class sControl *c = new sEntry(textmaxlen);

	if( !c ) return 0;

	if( addControl(c, x, y, xpad, ypad) &&  addObject(name, c)  )
	{
		return c;
	}
	else
	{
		delete c;
	}

	return 0;
}

class sControl* sWidget::createButton(int x, int y, const char *str, s_callback fun, void *data, const char *name)
{
	class sControl *c = new sButton(str, fun, data);

	if( !c ) return 0;

	if( addControl(c, x, y) &&  addObject(name, c)  )
	{
		return c;
	}
	else
	{
		delete c;
	}

	return 0;
}

class sControl* sWidget::createButtonEx(int x, int y, int xpad, int ypad, const char *str, s_callback fun, void *data, const char *name)
{
	class sControl *c = new sButton(str, fun, data);

	if( !c ) return 0;

	if( addControl(c, x, y, xpad, ypad) &&  addObject(name, c)  )
	{
		return c;
	}
	else
	{
		delete c;
	}

	return 0;
}

bool sWidget::delControl(class sControl *w)
{
	for(int i = 0; i < count_of( m_childs ); i++)
	{
		for(int j = 0; j < count_of( m_childs[0] ); j++)
		{
			if( m_childs[i][j] == w )
			{
				m_childs[i][j] == 0;
				unregistShortcut(w);
				w->setParent(0);
				keyDown();
				return true;
			}
		}
	}
	
	update();
	return false;
}

bool sWidget::setCurrent(class sControl *w)
{

	for(int i = 0; i < count_of( m_childs ); i++)
	{
		for(int j = 0; j < count_of( m_childs[0] ); j++)
		{
			if(m_childs[i][j] == w) return setCurrent(i, j);
		}
	}

	return false;
}

bool sWidget::setCurrent(int x, int y)
{
	if( !m_childs[x][y] || !m_childs[x][y]->getEnablefocus() ) return false;
	

	if( m_current )
	{
		m_current->setSelected(false);
		m_current->focusExchange();
	}

	m_current = m_childs[x][y];
	m_currentx = x;
	m_currenty = y;

	m_current->setSelected(true);
	m_current->focusExchange();

	if( m_ime && m_current->getEnableEdit() )
	{
//		areaUpdate(m_ime->getTop(), m_ime->getButtom());
		class sPyime *i = static_cast<class sPyime*>(m_ime);
		i->reset( getImeTop() );
		update();
	}

	return true;
}

void sWidget::destroy()
{     		
	getApplication()->setEventloop(false);
}

bool sWidget::addObject(const char *name, class sObject *object)
{
	if( !name ) return true;
	
	for(int i = 0; i < count_of(m_object); i++)
	{
		if( m_object[i].name == 0 )
		{
			m_object[i].name = name;
			m_object[i].object = object;
			return true;
		}
	}

	return false;
}

bool sWidget::delObject(const char *name)
{
	for(int i = 0; i < count_of(m_object); i++)
	{
		if( !strcmp(m_object[i].name, name) )
		{
			m_object[i].name = 0;
			m_object[i].object = 0;
			return true;
		}
	}

	return false;
}

bool sWidget::delObject(class sObject *object)
{
	for(int i = 0; i < count_of(m_object); i++)
	{
		if( m_object[i].object == object )
		{
			m_object[i].name = 0;
			m_object[i].object = 0;
			return true;
		}
	}

	return false;
}

class sObject* sWidget::getObject(const char *name)
{
	for(int i = 0; i < count_of(m_object); i++)
	{
		if( m_object[i].name && !strcmp(m_object[i].name, name) )
			return m_object[i].object;
	}

	return 0;
}

class sControl* sWidget::getControl(const char *name)
{
	return static_cast<class sControl*>( getObject(name) );
}

wchar_t* sWidget::getControlText(const char *name)
{
	class sControl *c = getControl(name);
	
	if(c) return c->getText();

	return 0;
}

char* sWidget::getControlString(const char *name)
{
	class sControl *c = getControl(name);
	
	if(c) return c->getString();

	return 0;
}

bool sWidget::setControlCurrent(const char *name)
{
	class sControl *c = getControl(name);

	if( c ) return setCurrent(c);

	return false;
}

void sWidget::setControlText(const char *name, const char *str)
{
	class sControl *c = getControl(name);

	if( c ) c->setText(str);
}


void sWidget::setControlText(const char *name, const wchar_t *str)
{
	class sControl *c = getControl(name);

	if( c ) c->setText(str);
} 
  
  
  
