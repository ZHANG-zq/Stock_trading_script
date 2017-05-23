#include "StdAfx.h"
#include "Moving_Average.h"
#include "workthread.h"
#include "math.h"


bool days20=false,days55=false;   //标记是否达到指定时间



Moving_Average::Moving_Average(void)
{
}


Moving_Average::~Moving_Average(void)
{
}


/*对速度要求不高，但内存要慎用*/
void Moving_Average::run_market(int num,char *code_str)
{
	if(pmydata==NULL)
		return;
	//第一天单独算
	//N *n1=new N;         //分别对应20天，55天
	//N *n2=new N;
	//n1->front=n1->next=n2->front=n2->next=NULL;
	//n1->value=n2->value=fabs(pmydata->Loprc - pmydata->Hiprc);  //第一天的N用当前交易日的最高价与最低价间的波幅
	pmydata++;
	//从第二天开始
	for(int t=2;t<=num;t++)
	{
		//double dn=calculate_N(pmydata);
		//set_N_full(n1,dn);
		//set_N_full(n2,dn);
		if(can_buy())            //没有持有股票
		{
					if(is_goldcross(t,5,20))
					{

			 			buy(pmydata);

					}
		}
		else
		{
					if(is_deathcross(t,5,20))
					{
						sell(pmydata);

					}
			}

		pmydata++;
	 }
	 //report(code_str);

}



bool Moving_Average::is_goldcross(int n,int small_period,int big_period)
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


bool Moving_Average::is_deathcross(int n,int small_period,int big_period)
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