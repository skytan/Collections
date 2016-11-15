#include "slist.h"
#include "sperr.h"

sList::sList(int row, int line)
{
	m_enable_focus = true;
	
	m_table = new sTable(row, line);
	if( !m_table ) perr("%s", "");
	
	m_table->setAutoUpdate( false );
	
	m_scrolled = new sScrolled();
	if( !m_scrolled ) perr("%s", "");
}

sList::~sList()
{
	if( m_table ) delete m_table;
	if( m_scrolled ) delete m_scrolled;
}

bool sList::setTitle(const char **str)
{
	return setTitle( (char**)str );
}

bool sList::setTitle(char **str)
{
	return m_table->setTitle(str);
}

void sList::prepare()
{
	m_table->setRect(m_left, m_top, m_right - m_scrolled->wide() - 2, m_buttom);
//	m_table->setLineHeight();
	
	m_scrolled->setBorder(m_right - m_scrolled->wide(), m_top, m_buttom - m_top);
}

void sList::adapt()
{	
	m_scrolled->setShowLine( m_table->viewLine() );
	m_scrolled->setCurrentLine( m_table->getCurrentLine() );
	m_scrolled->setCountLine( m_table->getCountLine() );
}

void sList::update ()
{	
	if( (m_buttom - m_top) < (m_table->getCountLine() * m_table->lineHeight() ) )
	{
		m_table->setRect(m_left, m_top, m_right - m_scrolled->wide() - 2, m_buttom);
		m_scrolled->setBorder(m_right - m_scrolled->wide(), m_top, m_buttom - m_top);
		
		adapt();
	
		m_table->update();
		m_scrolled->update();
	}
	else
	{
		m_table->setRect(m_left, m_top, m_right, m_buttom);
		
		m_table->update();
	}
}

void sList::keyEvent(class sEvent *e)
{
	m_table->keyEvent(e);
	
	
	update();
}

wchar_t* sList::getXyText(int x, int y)
{
	return m_table->getXyText(x, y);
}

bool sList::setXyText(int x, int y, char *str)
{
	return m_table->setXyText(x, y, str);
}

bool sList::insertLine(char **str, int y)
{
	if( !m_table->insertLine(str, y) ) return false;
	
	update();
	return true;
}

bool sList::insertLine(const char **str, int y)
{
	return insertLine( (char**)str, y);
}
