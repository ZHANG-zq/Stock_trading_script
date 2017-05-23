#pragma once


struct data{				
   double Opnprc;
   double Hiprc;
   double Loprc;
   double Clsprc;
   int Trdsta;
};        // 头文件出最好别声明全局变量,在.h文件中申明了一个全局常量，在编译时.cpp文件生成了一个，在include .h的另一cpp中又生成了一个造成错误。给那个全局变量加上static或者const，分别处理。

struct hold_data{
   char code[6];
   unsigned long long quantity;     //股票数量,可以乘以价格得出股票的市值 
   double N; 
   double Nnum;
   double price;  //持有股票的成交价（买入） 
   double Hiprc;  //从买入开始算的最高价，用与动态止盈

}; 


class account        
{
public: //必须的，难道现在默认是private？
	/*资产*/
	hold_data stock;                       //持有的股票的数据（现在只关注价格）
	unsigned long long cash;   //经过多次计算，最后cash与total_assets有些少偏差是正常的，所以不用混用
    unsigned long long total_assets;          //总资产
    unsigned long long initial_assets;  //初始资产
	/*业绩*/
	int profit_times;
	int loss_times;
	int no_loss_win_times;					 //虽然可能性很少，但还是将此考虑进去

	int continuous_profit;			 //连续盈利的最大次数，
	int continuous_loss;  
	int continuous_profit_temp;      //中间变量，用于比较
	int continuous_loss_temp;     

	unsigned long long profit;				 //总盈利的绝对额，
	unsigned long long loss;
	unsigned long long max_profit;			 //最大盈利，
	unsigned long long max_loss;

	double fee_rat;    //所有中间费用加起来的比率(限价交易）
	                    //（ 1‰－3‰（买进佣金），1‰－3‰（卖出佣金），1‰（卖出印花税）1‰（上海股票过户费）

    double sign_buyprice;   //发出买入信号时的价格 ，不等于成交价，要考虑费用，和不能及时买卖
    double sign_sellprice;   //发出买入信号时的价格 ，不等于成交价


	account::account();


};   //不要忘了加分号


