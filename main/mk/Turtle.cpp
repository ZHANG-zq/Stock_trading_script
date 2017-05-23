#include "StdAfx.h"
#include "Turtle.h"
#include "math.h"




Turtle::Turtle(void)
{
}


Turtle::~Turtle(void)
{
}


void Turtle::drop_firstTR(TR *t)
{
	while(t->front!=NULL)
		t=t->front;
	t->next->front=NULL;  //将下一位设为链头
	delete t;
}
void Turtle::drop_allTR(TR *t)
{
	TR *tn;                //tn为临时变量
	while(t->front!=NULL)
	{
		tn=t;
		t=t->front;
		delete tn;
	}
		delete t; //链头别忘了
}

/*
TR* Turtle::find_lastTR(TR *n)
{
	while(n->next!=NULL)
		n=n->next;
	return n;
}
*/

/*
double Turtle::calculate_TR(data *ppmydata)
{
	double d1=fabs(ppmydata->Loprc - pmydata->Hiprc);   //1.当前交易日的最高价与最低价间的波幅
	double d2=fabs((ppmydata-1)->Clsprc - pmydata->Hiprc);   //2.今天的最高价和昨天的收盘价之间的距离
	double d3=fabs((ppmydata-1)->Clsprc - pmydata->Loprc);   //3.今天的最低价和昨天的收盘价之间的距离
	if(d2<d3)
		d2=d3;
	if(d1<d2)
		d1=d2;
	return d1;
}


void Turtle::set_TR_unfull(TR *n,double dn)
{
		TR *tn=new TR;   //tn是临时变量
		tn->front=n;
		n=tn;
		n->value=dn;
}
*/

TR *Turtle::calculate_TR(int num,TR *t) //按址传递指针，指针的指向会发生改变，总会指向链尾
{

	 double d1=fabs((pmydata-1)->Loprc - (pmydata-1)->Hiprc);   //1.当前交易日的最高价与最低价间的波幅
	 double d2=fabs((pmydata-2)->Clsprc - (pmydata-1)->Hiprc);   //2.今天的最高价和昨天的收盘价之间的距离
	 double d3=fabs((pmydata-2)->Clsprc - (pmydata-1)->Loprc);   //3.今天的最低价和昨天的收盘价之间的距离
	 if(d2<d3)
		 d2=d3;
	 if(d1<d2)
		 d1=d2;
	
	TR *tn=new TR;   //tn是临时变量
	tn->value=d1;
	tn->front=t;
	t->next=tn;

	return tn;

}

double Turtle::get_N(TR *t)
{
    double aver=0;
	do
	{
		aver+=t->value;
	}while((t=t->front)!=NULL);
	aver/=15;
	return aver;

}

/*对速度要求不高，但内存要慎用*/
void Turtle::run_market(char *pcode,int num/*天数*/,int period/*周期*/,report_var *preport_var)
{
	if(pmydata==NULL)
		return;
	//第一天单独算N，第二天不算N，留给第三天来算
	TR *tr=new TR;
	tr->front=NULL;
	tr->value=fabs(pmydata->Loprc - pmydata->Hiprc);  //第一天的N用当前交易日的最高价与最低价间的波幅
	pmydata+=2;

	//从第三天开始
	double N;
	for(int t=3;t<=num;t++)
	{
		tr=calculate_TR(t, tr);
		if(t>15)      //第16天才开始交易，因为此时才集满15个tr来求N
		{

			N=get_N(tr);

			refresh_holding_data();//刷新持有股票的数据，主要看有没有触动止损/盈

			if(can_buy())     //没有持有股票
			{
						if(is_highest(t,period,pmydata)/* && not_SST(pmydata)*/)
						{
							/*
							ac.stock.N=N;
							int n=(ac.cash*0.05)/N;   //由于此处用全仓操作，所以折中写法，其实应该买入的数量都以N为单位
							ac.stock.Nnum=n;
							*/
			               
			 				buy(pmydata);

						}
			}
			else
			{
						if(is_lowest(t,period,pmydata) /*|| stop_lossing()|| stop_winning()*/) //卖出信号是敏感的，降低风险
						{
							sell(pmydata);   //必须先设置卖出信号ac.sign_sellprice
						}
			}

		    drop_firstTR(tr);   //一天过后，扔掉第一个N

		}
		pmydata++;   //其他位置别移动指针，只有这里移动
	 }
	 drop_allTR(tr);
	 report(pcode,period,preport_var);

	 pmydata=pmydata_old;
}




bool Turtle::is_highest(int n/*第几天*/,int period/*周期*/,data *ppmydata)
{

	if(n>=period)
	{
		    int i=1;
			double tHiprcp=pmydata->Hiprc/1.02;   //保证当天的最高价比前n-1天的最高价高2%(这也算是过滤器一部分）
			while(i<period && tHiprcp > (ppmydata-i)->Hiprc  ) 
				i++;

			if(i==period-1)       
			{
				i=1;               //当价格超过前n-1天的最高价的1%时，发出买入信号
				while(i<period)
				{
					if(ac.sign_buyprice<(ppmydata-i)->Hiprc)   
					   ac.sign_buyprice=(ppmydata-i)->Hiprc;
					i++;
				}
				ac.sign_buyprice*=1.02;
			    return true;
			}
	}
		return false;

}


bool Turtle::is_lowest(int n,int period,data *ppmydata)    //period为周期
{
	if(n>=period)
	{
		    int i=1;
			while(i<period && ppmydata->Clsprc < (ppmydata-i)->Clsprc)   //与买入不同，一低于前n-1天最低价发出信号
               i++;

			if(i==period-1)       
			{
				i=1;  
				ac.sign_sellprice=999999999; //无实际意义，技巧，便于比较
				while(i<period)
				{
					if(ac.sign_sellprice>(ppmydata-i)->Hiprc)   
					   ac.sign_sellprice=(ppmydata-i)->Hiprc;
					i++;
				}
			    return true;
			}
	 }
     		return false;
}

bool Turtle::stop_lossing()  //止损，还没有出场信号，但亏损已超过可承受最大风险——损失超过总资产的5%，以N为单位(以此来设定最大风险)
{
	if((ac.stock.price-pmydata->Loprc)>(4*ac.stock.N))   //固定止损点
	{
		ac.sign_sellprice=ac.stock.price-(4*ac.stock.N);
		return true;
	}
		/*
	if((ac.stock.price-pmydata->Loprc)*ac.stock.quantity > (ac.stock.quantity*ac.stock.price)*0.10)
	{

		ac.sign_sellprice = ac.stock.price*0.9;  //暂定,因为是全仓，所以总资产的5%损失，约等于股价的5%下跌
		return true;

	}
			*/
	return false;

}

bool Turtle::stop_winning()
{
	if(pmydata->Loprc < ac.stock.Hiprc*0.80 )
	{   
		ac.sign_sellprice = ac.stock.Hiprc*0.80 ; //暂定
		return true;           
	}
		return false;
}



void Turtle::refresh_holding_data()  
{
	if(ac.stock.quantity!=0) 
	{
		if(pmydata->Hiprc > ac.stock.Hiprc)
		 ac.stock.Hiprc=pmydata->Hiprc;

	}
}


bool Turtle::is_goldcross(int n,int small_period,int big_period)
{
	if(n>=big_period)
	{
		    float samll_average=0,big_average=0;
		    
			for(int i=0;i<small_period;i++)             //小的n天平均价格
			{
				samll_average+=(pmydata-i)->Clsprc;         //都用收盘价好像不太准确，当天不应该用收盘价，而是用最高价，因为要考虑被诱多的情况
			}
            samll_average/=small_period;

			
			for(int i=0;i<big_period;i++)               //大的n天平均价格
			{
				big_average+=(pmydata-i)->Clsprc;
			}
            big_average/=big_period;


			if(samll_average>big_average)
			{
				strtemp.Format("金叉出现\n");
			   //strtemp.Format("\t%d与%d天的平均移动线交叉(金叉)在%f价位出现\n",small_period,big_period,pmydata->Clsprc);
	           append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);
			   return true;
			}
	}
		return false;

}


bool Turtle::is_deathcross(int n,int small_period,int big_period)
{
	if(n>=big_period)
	{
		    float samll_average=0,big_average=0;
		    
			for(int i=0;i<small_period;i++)             //小的n天平均价格
			{
				samll_average+=(pmydata-i)->Clsprc;
			}
            samll_average/=small_period;

			
			for(int i=0;i<big_period;i++)               //大的n天平均价格
			{
				big_average+=(pmydata-i)->Clsprc;
			}
            big_average/=big_period;


			if(samll_average<big_average)
			{
				strtemp.Format("死叉出现\n");
			//strtemp.Format("\t%d与%d天的平均移动线交叉(死叉)在%f价位出现\n",small_period,big_period,pmydata->Clsprc);
	        append_text(hedit2,(LPTSTR)(LPCTSTR)strtemp);
			   return true;
			}
	}
		return false;

}