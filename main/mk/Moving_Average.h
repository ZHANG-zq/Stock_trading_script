#pragma once
#include "abstracttrade.h"


//移动平均线的交易策略
struct N{
	double value;
	N *front;
	N *next;
};

class Moving_Average :
	public AbstractTrade
{
public:
	Moving_Average(void);
	~Moving_Average(void);
	void run_market(int num,char *code_str);
	bool is_goldcross(int n,int small_period,int big_period);
	bool is_deathcross(int n,int small_period,int big_period);

};

