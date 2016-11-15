#include "sui.h"

static void messageback(class sObject *object, class sEvent *e, void *data)
{
	class sWidget *w = static_cast<class sWidget*>(data);
	
	w->destroy();
}

void messageBox(const char *str)
{
	class sWidget *w = new sWidget(3, 3);

	w->createLabelEx(0, 0, 3, 2, str);
	w->createButton(1, 2, "确定", S_CALLBACK(&messageback), w);

	w->show();
}

static void yes_click(class sObject *object, class sEvent *e, void *data)
{
	*(int*)data = 1;
	
	class sControl *c = static_cast<class sControl*>(object);
	sWidget *w = static_cast<class sWidget*>( c->getParent() );
	
	w->destroy();
}

static void no_click(class sObject *object, class sEvent *e, void *data)
{
	*(int*)data = 0;
	
	class sControl *c = static_cast<class sControl*>(object);
	sWidget *w = static_cast<class sWidget*>( c->getParent() );
	
	w->destroy();
}

int yesNoBox(const char *str)
{
	int retval = -1;
	class sWidget *w = new sWidget(4, 3);
	
	w->createLabelEx(0, 0, 4, 2, str);
	w->createButton(2, 2, "取消", S_CALLBACK(no_click), &retval);
	w->createButton(1, 2, "确定", S_CALLBACK(yes_click), &retval);
	
	w->show();
	return retval;
}
