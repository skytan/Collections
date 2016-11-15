#include "window_sale.h"
#include "common/goods.h"
#include "common/invoice.h"
#include "common.h"

struct invoice invoceTest = {
	1, 1, 1, 1, 1,
	0,0,
	0, 0, 0, "", "",
	"121000921201", "00000000",
	"01", "2010-6-1",
	"1455 4910 2024 3978 6005",
	"国家税务局",
	"纳税人单位名称",
	"纳税号",
	"机器编号"
};

int sale_keysale(class sObject *w, class sEvent *e, void *data)
{
	class sWidget *window = (class sWidget*)w;
	class sList *list = (class sList *)data;
	struct goods good;
	
	strcpy(good.szEntry, window->getControlString("input") );
	
	if( goods_read( &good ) )
			return pret(-1);
	
	char *str[] = {good.szEntry, good.szName, 0};
	list->insertLine( str );
	
	return 0;
}

int sale_keyenter(class sObject *w, class sEvent *e, void *data)
{
	class sWidget *window = (class sWidget*)w;
	class sList *list = (class sList *)data;
	int line = list->getCurrentLine();
	char *str = window->getControlString("input");
	
	list->setXyText(2, line, str);
	
	return 0;
}

int sale_keyprint(class sObject *w, class sEvent *e, void *data)
{
	class sWidget *window = (class sWidget*)w;
	class sList *list = (class sList *)data;
	struct goods *pgood = (struct goods *)calloc( sizeof(struct goods), list->getCount() );
	
	for(int i = 0; i < list->getCount(); i++)
	{		
		strcpy( pgood[ i ].szEntry, (const char*)list->getXyText(0, i) );
		if(  goods_read( pgood + i ) )
		{
			free( pgood );
			return pret(-1);
		}
		
		pgood[ i ].iPrice = atoi( (const char*)list->getXyText(0, i) ) * 100;
	}		
	
	invoceTest.iGoodsNumber = list->getCount();
	invoceTest.ptrGoods = pgood;
	
	invoice_print( &invoceTest );
	return 0;
}

int sale_key_event(class sObject *w, class sEvent *e, void *data)
{	
	switch( e->getValue() )
	{
		case KEY_SALE:
			return sale_keysale(w, e, data);
		case KEY_PRINT:
			return sale_keyprint(w, e, data);
		case KEY_ENTER:
			return sale_keyenter(w, e, data);
		default:
			break;
	}
}

void saleMainWindow()
{
	class sWidget *window = new sWidget(6, 4, 0, 0);
	class sList *list = new sList(3, 16);
	const char *title1[] = {"项目", "数量", "金额"};
	const char *title2[] = {"xianmu2", "shuliang2", "金额"};
	const char *title3[] = {"xianmu3", "shuliang3", "jinye3"};
	const char *title4[] = {"xianmu4", "shuliang4", "jinye4"};
	
	window->createLabel(0, 0, "总额");
	window->createLabelEx(1, 0, 2, 1, "0.00");
	window->createLabel(3, 0, "输入");
	window->createEntryEx(4, 0, 2, 1, 8, "input");
	
	window->addControl(list, 0, 1, 6, 3);

	list->setTitle( title1 );
//	list->insertLine( title2 );
//	list->insertLine( title3 );
//	list->insertLine( title4 );
	
	window->setCallback( S_CALLBACK(&sale_key_event), list );
	window->show();
}
