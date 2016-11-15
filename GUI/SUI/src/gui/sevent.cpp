#include "sevent.h"

sEvent::sEvent(sEvent *e)
{
	if(e)
	{
		type = e->getType();
		value = e->getValue();
		mask = e->getMask();
	}
	else
	{
		type = 0;
		value = 0;
		mask = 0;
	}
	
	busy = false;
}

sEvent::~sEvent()
{
}

int sEvent::getType() const
{
	return type;
}

int sEvent::getValue() const
{
	return value;
}

int sEvent::getMask() const
{
	return mask;
}

void sEvent::setMask (int x)
{
      mask = x;
}

void sEvent::setType(int x)
{
      type = x;
}

void sEvent::setType(const class sEvent *e)
{
      type = e->getType();
      value = e->getValue();
}

void sEvent::setValue(int x)
{
      value = x;
}

void sEvent::setValue(const class sEvent *e)
{
      type = e->getType();
      value = e->getValue();
}

void sEvent::beginSelect()
{
      busy = true;
}

void sEvent::endSelect ()
{
      busy = false;
}

class sEvent* sEvent::select()
{
}

