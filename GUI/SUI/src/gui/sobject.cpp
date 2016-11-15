#include "sobject.h"
#include "sconfig.h"

class sApplication sObject::s_app;
class sFramebuffer sObject::s_fb;

sObject::sObject()
{
	parent = 0;
	
}


int sObject::getXres()
{
#if SCONFIG_SCREEN_XRES
	return SCONFIG_SCREEN_XRES;
#else
	return s_fb.getFbXres();
#endif
}


int sObject::getYres()
{
#if SCONFIG_SCREEN_YRES
	return SCONFIG_SCREEN_YRES;
#else
	return s_fb.getFbYres();
#endif
}

void sObject::setPoint(int x, int y, int val)
{
	s_fb.setXy(x, y, val);
}

void sObject::reversePoint(int x, int y)
{
	s_fb.reversePoint(x, y);
}