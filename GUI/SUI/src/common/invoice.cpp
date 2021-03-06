#include "invoice.h"
#include "../perr.h"
#include "mpm78m_printer.h"
#include "../common.h"
#include "goods.h"
#include <malloc.h>

char* int2h(int i)
{
	static char buf[16];
	
	sprintf(buf, "%d.%d", i / 100, i % 100);
	
	return buf;
}

char* l2g(char *number)
{
	return 0;
}
	
	
int invoice_total(struct invoice *ptrInc)
{
	int total = 0;
	struct goods *pgood;	
	
	for(int i = 0; i < ptrInc->iGoodsNumber; i++)
	{
		pgood = ptrInc->ptrGoods + i;
		total += pgood->iPrice * pgood->iQuantity;
	}
	
	return total;
}


int invoice_goods_print(int printfd, struct invoice *ptrInc)
{
	char buf[512];
	struct goods *pgood;
	
	for(int i = 0; i < ptrInc->iGoodsNumber; i++)
	{
		pgood = ptrInc->ptrGoods + i;
 		sprintf(buf, "%-8s%-8d%-8d%-8d", pgood->szName, pgood->iPrice, pgood->iQuantity, pgood->iPrice * pgood->iQuantity );
		mpm78m_print_utf8(printfd, (unsigned char*)buf);
	}
	
	return 0;
}

int invoice_fill(struct invoice *ptrInc)
{
	config_query_machine_sn( ptrInc->uchFiscalCashRegister );
	ptrInc->iTotal = invoice_total( ptrInc );
	
	return 0;
}

int invoice_print(struct invoice *ptrInc)
{
	unsigned char buf[512];
	
	invoice_fill( ptrInc );	 
	 
	int printfd = open_printer_com();
	if( printfd < 0 ) return -1;
	
	mpm78m_read_black_mark(printfd);
	
	sprintf((char*)buf, "%s%s", "发票代码:", ptrInc->szBaseCode);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "发票号码:", ptrInc->szNumber);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "机打号码:", "00000002");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "机器编号:", ptrInc->uchFiscalCashRegister);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "收款单位:", ptrInc->uchTaxpayerDepartmentName);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "税务登记号:", ptrInc->uchTaxNo);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%-12s%s%s", "开票日期:", ptrInc->szTime, "收款员:", ptrInc->szCheckerCode);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "付款单位:", "个人");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	mpm78m_feed_on_line(printfd, 4);
	
	invoice_goods_print(printfd, ptrInc);
	mpm78m_feed_on_line(printfd, 8 - ptrInc->iGoodsNumber);
	
	sprintf((char*)buf, "%s%s", "小写合计:", "1.00");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "大写合计:", "壹圆整");
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	sprintf((char*)buf, "%s%s", "税控码:", ptrInc->szFiscalCode);
	mpm78m_print_utf8(printfd, buf);
	mpm78m_feed_mm(printfd, 10);
	
	close_printer_com(printfd);
}

int invoice_write(struct invoice *prtInc)
{
	FILE *fp;
	int seek;
	struct goods	*ptrGoods = prtInc->ptrGoods;

	if( !(fp = fopen(INVOICE_GOODS_FILE, "a")) )
		return pret(-1);

	seek = ftell(fp);
	if( fwrite(&prtInc->ptrGoods, prtInc->iGoodsNumber, sizeof(struct goods), fp) != sizeof(struct goods) )
	{
		fclose(fp);
		return pret(-2);
	}

	fclose(fp);

	if( (fp = fopen(INVOICE_FILE, "a")) < 0 )
		return pret(-4);

	prtInc->ptrGoods = (struct goods*)seek;

	if( fwrite(prtInc, sizeof(struct invoice), 1, fp) != 1 )
	{
		fclose(fp);
		prtInc->ptrGoods = ptrGoods;
		return pret(-5);
	}

	fclose(fp);
	prtInc->ptrGoods = ptrGoods;
	
	return 0;
}

int invoice_fill_goods(struct invoice *prtInc)
{
	FILE *pf;
	
	if( !(pf = fopen(INVOICE_GOODS_FILE, "r")) )
		return pret(-1);
	
	if( fseek(pf, (long)prtInc->ptrGoods, SEEK_SET) )
	{
		fclose( pf );
		return pret(-2);
	}
	
	if( !( prtInc->ptrGoods = (struct goods*) malloc(prtInc->iGoodsNumber * sizeof(*(prtInc->ptrGoods) )) ) )
	{
		fclose( pf );
		return pret(-3);
	}
	
	if( fread(prtInc->ptrGoods, prtInc->iGoodsNumber, sizeof(*(prtInc->ptrGoods) ), pf) != sizeof(*(prtInc->ptrGoods) ) )
	{
		free( prtInc->ptrGoods );
		fclose( pf );
		return pret(-4);
	}
	
	fclose(pf);
	return 0;
}

int invoice_read(struct invoice *prtInc)
{
	struct invoice inv;
	FILE *pf;
	
	if( !(pf = fopen(INVOICE_FILE, "r")) )
		return pret(-1);
	
	while( fread(&inv, sizeof(inv), 1, pf) == 1 )
	{
		if( strcmp(prtInc->szBaseCode, inv.szBaseCode) || strcmp(prtInc->szNumber, inv.szNumber) )
			continue;
		
		memcpy(prtInc, &inv, sizeof(inv));
		fclose(pf);
		
		return  invoice_fill_goods(prtInc);
	}
	
	fclose( pf );
	return pret(-2);
		
}

