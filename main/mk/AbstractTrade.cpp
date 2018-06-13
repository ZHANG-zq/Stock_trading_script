#include "StdAfx.h"
#include "AbstractTrade.h"
#include "resource.h"  
#include "workthread.h"

data *pmydata_old=NULL,*pmydata=NULL;


AbstractTrade::AbstractTrade(void)
{


}


AbstractTrade::~AbstractTrade(void)
{
}



bool AbstractTrade::can_buy()
{
	if(ac.stock.quantity==0)  
	   return true;
	else
	   return false;
 
}

bool AbstractTrade::can_sell()
{
	if(ac.stock.quantity!=0)
	   return true;
	else
	   return false;
 
}

void AbstractTrade::buy(data* ppdata)
{
	ac.stock.price=ac.sign_buyprice*ac.fee_rat;
        ac.stock.quantity=ac.cash/ac.stock.price;      
	ac.stock.quantity/=100;   //最低买卖100股
	ac.stock.quantity*=100; 
       

	#ifdef _DEBUG
	if(ac.cash<(ac.stock.price*ac.stock.quantity))
	{
		AfxMessageBox("不够钱买"); 
	}
	#endif

	ac.cash-=(ac.stock.price*ac.stock.quantity);  

	strtemp.Format("在%lf价位买入%I64u股,成交价%lf\n",ac.stock.price,ac.stock.quantity,ac.stock.price);
	append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);



}

void AbstractTrade::sell(data* ppdata)   //将卖出信号的价格当作成交价，期间的误差在中间费用里体现
{
	strtemp.Format("在%lf价位卖出%I64u股\n\n\n",ac.sign_sellprice,ac.stock.quantity);
	append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);

	if (ac.sign_sellprice>ac.stock.price)
	{ 
                unsigned long long d;
		d=(ac.sign_sellprice-ac.stock.price)*ac.stock.quantity;
		ac.profit+=d;
		ac.total_assets+=d;
                ac.profit_times++;

		/*记录连续盈利的最大次数*/
		ac.continuous_profit_temp++;
		ac.continuous_loss_temp=0;
		if(ac.continuous_profit<ac.continuous_profit_temp)
		    ac.continuous_profit=ac.continuous_profit_temp;

	        /*记录最大盈利*/
	        if(ac.max_profit<d)
		    ac.max_profit=d;

		strtemp.Format("本次盈利%I64u\t累计盈利次数%d\t累计盈利%I64u\t总资产%I64u\n\n\n",d,ac.profit_times,ac.profit,ac.total_assets);
	        append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);

	}

	/*亏损*/
	if (ac.sign_sellprice<ac.stock.price)
	{ 
		unsigned long long d;
		d=(ac.stock.price-ac.sign_sellprice)*ac.stock.quantity;
		ac.loss+=d;
		ac.total_assets-=d;
		ac.loss_times++;

		/*连续亏损的最大次数*/
		ac.continuous_loss_temp++;
		ac.continuous_profit_temp=0;
		if(ac.continuous_loss<ac.continuous_loss_temp)
		    ac.continuous_loss=ac.continuous_loss_temp;

	        /*记录最大亏损*/
		if(ac.max_loss<d)
		    ac.max_loss=d;

		strtemp.Format("本次亏损%I64u\t累计亏损次数%d\t累计亏损%I64u\t总资产%I64u\n\n\n",d,ac.loss_times,ac.loss,ac.total_assets);
	        append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);


	}
	
	if(ac.sign_sellprice==ac.stock.price)  /*无亏损，无利润的神奇状态*/
	{
		ac.no_loss_win_times+=1;
		strtemp.Format("\n\n*********本次交易无盈亏**********\t总资产%I64u",ac.total_assets);
	        append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);
	}

	append_text(hedit2,"\n\n");
	ac.cash+=ac.sign_sellprice*ac.stock.quantity;   //回笼现金
	clear_holdingstock();

}


void AbstractTrade::clear_holdingstock()
{
	ac.sign_buyprice=0;
	ac.sign_sellprice=0;
	ac.stock.Hiprc=0;
	ac.stock.price=0;
	ac.stock.quantity=0;
	ac.stock.N=0;
	ac.stock.Nnum=0;
}


bool AbstractTrade::not_SST(data* ppdata)
{
	if(ppdata->Trdsta==3 || ppdata->Trdsta==6)  //sst股票不买
		return false;
	return true;
}



void AbstractTrade::report(char *pcode,int period,report_var *preport_var)
{         //计算数据

	if(can_sell())
	{   
		ac.sign_sellprice=(pmydata-1)->Clsprc;
		sell((pmydata-1)); 
	}         


	if(ac.profit_times+ac.loss_times+ac.no_loss_win_times!=0)     
	{       
		preport_var->icount++;
		preport_var->total_assets+=ac.total_assets;      
		preport_var->profit_times+=ac.profit_times;
		preport_var->loss_times+=ac.loss_times;
		preport_var->profit+=ac.profit;
		preport_var->loss+=ac.loss;
	}
	else if(preport_var->icount==0)
	{   
		strtemp.Format("代码%s\n\n周期%d天\n\n总资产余额%I64u\n\n盈利次数%d\n\n最大连续盈利次数%d\n\n亏损次数%d\n\n最大连续亏损次数%d\n\n盈利总额%I64u\n\n最大盈利额%I64u\n\n亏损总额%I64u\n\n最大亏损额%I64u\n\n",pcode,period,ac.total_assets,ac.profit_times,ac.continuous_profit,ac.loss_times,ac.continuous_loss,ac.profit,ac.max_profit,ac.loss,ac.max_loss);
		append_text(hedit1,(LPTSTR)(LPCTSTR)strtemp);
	        strtemp.Format("计算0只股票\n\n周期%d天\n\n",period);
	        append_text(hedit3,(LPTSTR)(LPCTSTR)strtemp);   
		return;
	}

	int aver_profit_times=0,aver_loss_times=0;
	unsigned long long aver_total_assets=0,aver_profit=0,aver_loss=0;

	aver_loss_times=preport_var->loss_times/preport_var->icount;     
	aver_profit_times=preport_var->profit_times/preport_var->icount;            
	aver_total_assets=preport_var->total_assets/preport_var->icount; 
	aver_profit=preport_var->profit/preport_var->icount; 
	aver_loss=preport_var->loss/preport_var->icount; 

	strtemp.Format("代码%s\n\n周期%d天\n\n总资产余额%I64u\n\n盈利次数%d\n\n最大连续盈利次数%d\n\n亏损次数%d\n\n最大连续亏损次数%d\n\n盈利总额%I64u\n\n最大盈利额%I64u\n\n亏损总额%I64u\n\n最大亏损额%I64u\n\n",pcode,period,ac.total_assets,ac.profit_times,ac.continuous_profit,ac.loss_times,ac.continuous_loss,ac.profit,ac.max_profit,ac.loss,ac.max_loss);
	append_text(hedit1,(LPTSTR)(LPCTSTR)strtemp);

	strtemp.Format("计算%d只股票\n\n周期%d天\n\n平均总资产余额%I64u\n\n平均盈利次数%d\n\n平均亏损次数%d\n\n平均盈利总额%I64u\n\n平均亏损总额%I64u",preport_var->icount,period,aver_total_assets,aver_profit_times,aver_loss_times,aver_profit,aver_loss);
	append_text(hedit3,(LPTSTR)(LPCTSTR)strtemp);

	//strtemp.Format("计算%d只股票\n\n周期%d天\n\n平均总资产余额%I64u\n\n平均盈利次数%d\n\n平均亏损次数%d\n\n平均盈利总额%I64u\n\n平均亏损总额%I64u",preport_var->icount,period,aver_total_assets,aver_profit_times,aver_loss_times,aver_profit,aver_loss);
	//append_text(hedit3,(LPTSTR)(LPCTSTR)strtemp);
	//SetWindowText(hedit3,strtemp);

	//SetWindowText(hedit2," ");

}

