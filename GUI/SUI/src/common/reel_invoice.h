/******************************************************************
** 文件名: goods.h
** Copyright (c) 2010 桑达商用 技术开发部
** 创建人: 陈晋飞
** 日　期: 2011年3月21日
** 修改人:
** 日　期:
** 功  能: 发票的定义
**         
**         
**         
**         
** 用  途: 
**
** 版　本: 0.1
********************************************************************/

#ifndef  REEL_INVOICE_H
#define  REEL_INVOICE_H

//单卷发票数据
struct  reel_invoice
{
	int  	uiValidInvoiceNum;    				//正常发票总数
	int  	uiReturnInvoiceNum;   				//退票总数
	int  	uiInvalidInvoiceNum;  				//废票总
	int	ulTotalMoney; 			//正常发票总金额
	int	ulReturnTotalMoney;			//退票总金额

	char 	szName[204];		//税人单位名称40
	char 	szTaxNo[24];						//纳税人编码8
	char 	chFiscalCashRegister[16];			//税控收款机机器编号8	
	char 	szInvoiceBaseCode[20];				//发票代码
	char 	szInvoiceStartNo[12];  				//发票起始号
	char 	szInvoiceEndNo[12];    				//发票终止号

	char 	szStartDate[12];						//开始使用日期 2002-01-01
	char 	szEndDate[12];						//结束使用日期
}__attribute__ ((__packed__));;

typedef struct reel_invoice struReelInvoice;


#endif
