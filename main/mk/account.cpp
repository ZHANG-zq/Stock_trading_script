#include "StdAfx.h"
#include "account.h"
#include "resource.h"   //为了使用控件id
#include "workthread.h"




 

account::account()
{
	stock.code[0]=' ';
	stock.quantity=0;
    stock.price=0;
	stock.N=0;
	stock.Nnum=0;
	stock.Hiprc=0;

	max_profit=max_loss=profit=loss=0;
	continuous_profit_temp=continuous_loss_temp=continuous_loss=continuous_profit=profit_times=loss_times=no_loss_win_times=0;
	cash=initial_assets=total_assets=1000000;           //初始资金1000000

	fee_rat=1.01;    //百分之二的费用经过多次交易后对业绩的影响很大。
	sign_buyprice=sign_sellprice=0;

}