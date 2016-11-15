#include "window_goods.h"
#include "common/goods.h"
#include "common.h"

int gui_to_goods(class sWidget *w, struct goods* pgood)
{
	class sControl *entry;

	pgood->iPrice = atoi( w->getControlString("price") );
	
	strcpy( pgood->szEntry, w->getControlString("entry") );
	strcpy( pgood->szName, w->getControlString("name") );
	strcpy( pgood->szType, w->getControlString("type") );
	strcpy( pgood->szModel, w->getControlString("model") );
	
	return 0;
}

void goods_gui_clear(class sWidget *w)
{
	class sControl *entry;

	w->setControlText("entry", "");
	w->setControlText("price", "");
	w->setControlText("name", "");
	w->setControlText("type", "");
	w->setControlText("model", "");
	
}


int goods_to_gui(struct goods *pgood, class sWidget *w)
{
	char buf[16];
	
	sprintf(buf, "%d", pgood->iPrice);

	w->setControlText("entry", pgood->szEntry);
	w->setControlText("price", buf);
	w->setControlText("name", pgood->szName);
	w->setControlText("type", pgood->szType);
	w->setControlText("model", pgood->szModel);

	return 0;
}


int set_key_enter(class sWidget *w)
{
	struct goods good;
	class sControl *entry;
	wchar_t *str;

	str = w->getControlText("entry");	
	if( !str[0] )
	{
		w->setControlCurrent("entry");
		return 0;
	}

	str = w->getControlText("name");
	if( !str[0] )
	{
		strcpy(good.szEntry, w->getControlString("entry") );
		if( goods_read( &good ) )
			return pret(-1);
		
		return goods_to_gui(&good, w);
	}

	gui_to_goods(w, &good);
	if( goods_write( &good ) )
		return pret(-2);

	messageBox("商品保存成功");
	goods_gui_clear(w);
	w->setControlCurrent("entry");
}

void setCallback(class sObject *w, class sEvent *e, void *data)
{
	int c = e->getValue();

	switch(c)
	{
		case KEY_ENTER:
			set_key_enter( static_cast<class sWidget*>(data) );
			break;
		default:
			break;
	}
}

void goodsSetWindow()
{	
	class sWidget *wset = new sWidget(6, 3);
	wset->setCallback(S_CALLBACK(&setCallback), wset);
	
	wset->createLabel(0, 0, "编码");
	wset->createEntryEx(1, 0, 2, 1, 32, "entry");

	wset->createLabel(3, 0, "单价");
	wset->createEntryEx(4, 0, 2, 1, 8, "price");

	wset->createLabel(0, 1, "名称");
	wset->createEntryEx(1, 1, 5, 1, 8, "name");

	wset->createLabel(0, 2, "规格");
	wset->createEntryEx(1, 2, 2, 1, 8, "type");

	wset->createLabel(3, 2, "单位");
	wset->createEntryEx(4, 2, 2, 1, 8, "model");
	
	wset->show();
	
}


void goodsMainWindow()
{
	class sWidget *wmain = new sWidget(1, 3);

	wmain->createButton(0, 0, "1.商品设置", S_CALLBACK(&goodsSetWindow) );
	wmain->createButton(0, 1, "2.导入商品");
	wmain->createButton(0, 2, "3.导出商品");
	
	wmain->show();
}
