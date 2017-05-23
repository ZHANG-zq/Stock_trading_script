#pragma once
#include "account.h"
#include "workthread.h"

class AbstractTrade     //基类
{
public:
	account ac;
	AbstractTrade(void);
	~AbstractTrade(void);
	 
	bool can_buy();      // 具体的策略实现是否可以买
	bool can_sell();     // 具体的策略实现是否可以卖
	void buy(data* ppdata);
	void sell(data* ppdata);

	

    /*
	void GetMaxAmount();  // 得到可以买的最大数量
	void GetBuyPrice();   // 得到购买的价格
	void GetSellPrice();  // 得到卖出的价格
	void GetAvailableAmount();  // 得到当前可以卖出的的股票数
	void GetAmount();    // 得到总的数量
	void IsHaveStockToSell();   // 是否有股票可以卖
	void IsHaveTheStockAlready();  // 是否已经有这个股票了 (必须有数量)
	*/

	CString strtemp;

	void report(char *pcode,int period,report_var *preport_var);   //输出结果

	void report_new(char *code_str);

	void clear_holdingstock();   //清空账户数据
	bool not_SST(data* ppdata);   //检验是否st股票
}; 


extern  data *pmydata_old;   //不能直接在这里声明静态变量，不然在其他文件也会生成静态变量，导致有多个。
extern  data *pmydata;       //当一个源程序由多个源文件组成时，非静态的全局变量在各个源文件中都是有效的。 而静态全局变量则限制了其作用域， 即只在定义该变量的源文件内有效

