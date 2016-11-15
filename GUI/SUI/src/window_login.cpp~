#include "window_login.h"
#include "sui.h"

static void cancel_click(class sObject *object, class sEvent *e, void *data)
{
	sWidget *w = static_cast<class sWidget*>( object->getParent() );
	
	w->destroy();
}

static void ok_click(class sObject *object, class sEvent *e, void *data)
{
	*(int*)data = 0;
	
	sWidget *w = static_cast<class sWidget*>( object->getParent() );
	
	w->destroy();
}

int login()
{
	int retval = -1;
	class sWidget *window = new sWidget(2, 3);
	
	window->createLabel(0, 0, "user");
	window->createEntry(1, 0, 8, "user");
	
	window->createLabel(0, 1, "password");
	window->createEntry(1, 1, 8, "password");
	
	window->createButton(0, 2, "cancel", S_CALLBACK(&cancel_click), &retval);
	window->createButton(1, 2, "ok",  S_CALLBACK(&ok_click), &retval);
	
	window->show();
	return retval;
}