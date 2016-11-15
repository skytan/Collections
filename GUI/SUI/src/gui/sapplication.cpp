#include "sapplication.h"
#include "swidget.h"
#include "scontrol.h"


sApplication::sApplication()
{
	m_stack_point = 0;
	m_event_loop = true;
	bzero(m_stack, sizeof(m_stack));
	
	m_event = new sKeyEvent();
}

sApplication::~sApplication()
{
	if( m_event ) delete m_event;
}

bool sApplication::pushWidget(class sObject *w)
{
	if( m_stack_point >= count_of(m_stack) )
	{
		perr("%s", "stack overflow");
		return false;
	}
	
	m_stack[ m_stack_point++ ] = w;
	
	return true;
}

class sObject* sApplication::popWidget()
{
	if( --m_stack_point <= 0 ) return 0;
	
	return m_stack[ m_stack_point - 1 ];
}


bool sApplication::execWidget(class sWidget *w)
{
	if( !pushWidget(w) ) return false;

	clearEvent();
	sendUpdate();
	beginEventloop();

	popWidget();

	clearEvent();
	sendUpdate();
	m_event_loop = true;
	
	return true;
}


void sApplication::sendUpdate()
{
	m_second_event.setType(EVENT_TYPE_UPDATE);
}

void sApplication::sendMeaaage(class sEvent *e)
{
	m_second_event.setType( e );
}

void sApplication::sendMeaaage(int type, int value)
{
	m_second_event.setType( type );
	m_second_event.setValue( value );
}

void sApplication::clearEvent()
{
	m_second_event.setType(0);
	m_event->setType(0);
}

void sApplication::sendEvent(class sObject *w, class sEvent *e)
{
	switch( e->getType() )
	{
		case EVENT_TYPE_KEY:
			if( e->getValue() == 0x3 ) exit();
			w->keyEvent(e);
			if( e->getValue() == KEY_ENTER )  w->click(e);			
			break;
		case EVENT_TYPE_TIMER:
			w->timer(e);
			break;
		case EVENT_TYPE_TEXTCHANGE:
			w->textExchange();
			break;
		case EVENT_TYPE_UPDATE:
			w->update();
			break;
		case EVENT_TYPE_QUIT:   
			exit(e->getValue());
			break;
		default:
			break;
	}
}
			

void sApplication::sendEvent(class sEvent *e)
{
#if EVENT_TIME
	struct timeval tvin;
	struct timeval tvout;
#endif
	int point = m_stack_point;
	class sWidget *w = static_cast<class sWidget *>(m_stack[ point - 1 ]);
	class sControl *child = w->getCurrent();
	class sControl *ime = w->getIme();
 
	if( point <=0 || !w ) return;
#if EVENT_TIME	
	gettimeofday(&tvin, 0);
#endif  
	m_stack[ point - 1 ]->event(e);
  
	if( child &&  e->getType())
	{	    
		if( m_event_loop && point == m_stack_point && child->getIsedit() && ime &&  e->getType() == EVENT_TYPE_KEY ) ime->keyEvent(e);
		
		if( m_event_loop && point == m_stack_point &&  e->getType() ) sendEvent(child, e);
	}

	if( m_event_loop && point == m_stack_point &&  e->getType() ) sendEvent(m_stack[ point - 1 ], e);
#if EVENT_TIME	
	gettimeofday(&tvout, 0);
	
	printf("\tsend message cost time(micro seconeds):%u\n", tvout.tv_usec - tvin.tv_usec);
#endif
}

void sApplication::beginEventloop()
{
	class sEvent *e;
	class sEvent tmp;
	
	m_event_loop = true;

	while( m_event_loop && m_stack_point > 0 )
	{	  
		if( m_second_event.getType() )
		{
			tmp.setType( &m_second_event );		   
			m_second_event.setType(0);
			
			sendEvent( &tmp );		     
			continue;
		}

		if( e = m_event->select() ) sendEvent( e );
	}  

	if( m_stack_point <= 0 ) print("%s", "stack_point <= 0");
}

void sApplication::setEventloop(bool b)
{
	m_event_loop = b;
}

void sApplication::exit(int e)
{
	for(int i = m_stack_point-1; i >= 0; i--)
	{
		delete m_stack[i];
		m_stack[i] = 0;
	}
  
	delete m_event;
	m_event = 0;
  
	::exit(e);
}


bool sApplication::getEventloop()
{
	return m_event_loop;
}

