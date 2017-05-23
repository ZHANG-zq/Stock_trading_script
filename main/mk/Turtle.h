#pragma once
#include "abstracttrade.h"
#include "workthread.h"

struct TR{       //true range
	double value;
	TR *front;
	TR *next;
};

class Turtle :
	public AbstractTrade
{

public:
	Turtle(void);
	~Turtle(void);
	void run_market(char *pcode,int num,int period,report_var *preport_var);

	bool is_goldcross(int n,int small_period,int big_period);
	bool is_deathcross(int n,int small_period,int big_period);


    TR *calculate_TR(int num,TR *t);
	double get_N(TR *t);
	TR* find_lastTR(TR *t);
	void drop_firstTR(TR *t);
	void drop_allTR(TR *t);

	bool is_highest(int n,int period,data *ppmydata);
	bool is_lowest(int n,int period,data *ppmydata);
	bool stop_lossing();
	bool stop_winning();
	void refresh_holding_data();



};



