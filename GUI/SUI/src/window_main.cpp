#include "window_main.h"
#include "window_sale.h"
#include "window_goods.h"
#include "window_login.h"
#include "common.h"


void main_key_event(class sObject *object, sEvent * event, void* data)
{
	if( 0 > login() ) return;
	
	yesNoBox("hello");
}


void table_click(class sObject *object, sEvent * event, void* data)
{
	class sWidget *window  = new sWidget(1, 1);
	class sTable *t = new sTable(3);
//	class sList *t = new sList(0, 0, 190, 60, 3);
	
	const char *p1[] = {"11", "12", "13"};
	const char *p2[] = {"21", "22", "23"};
	const char *p3[] = {"31", "32", "33"};
	const char *p4[] = {"41", "42", "43"};
	const char *p5[] = {"51", "52", "53"};
	
	t->insertLine( (char**)p1 );
	t->insertLine( (char**)p2 );
	t->insertLine( (char**)p3 );
	t->insertLine( (char**)p4 );
	t->insertLine( (char**)p5 , 0);
	
	window->addControl(t);
	
	window->show();
}

void query_click(class sObject *object, sEvent * event, void* data)
{
	class sWidget *window  = new sWidget(1, 1);
	class sControl *t = new sScrolled(0, 0, 60);
	
	window->addControl(t);
	
	window->show();
}


void create_main_window()
{
	sWidget *window;

	window = new sWidget(3, 3);
	
	window->createButton(0, 1, "1.开票", S_CALLBACK(&saleMainWindow));
	window->createButton(1, 1, "2.管理", S_CALLBACK(&goodsMainWindow));
	window->createButton(2, 1, "3.查询", S_CALLBACK(&query_click));
	window->createButton(0, 2, "4.报表", S_CALLBACK(&table_click));
	window->createButton(1, 2, "5.上传", S_CALLBACK(&main_key_event));
	window->createButton(2, 2, "6.关机");
	
	window->show();
}
