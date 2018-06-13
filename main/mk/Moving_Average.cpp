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



void Moving_Average::run_market(int num,char *code_str)
{
	if(pmydata==NULL)
		return;
	pmydata++;
	for(int t=2;t<=num;t++)
	{
		if(can_buy())          
		{
			if(is_goldcross(t,5,20))
				buy(pmydata);
		}
		else
		{
			if(is_deathcross(t,5,20))
				sell(pmydata);
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
		for(int i=0;i<small_period;i++)            
			samll_average+=(pmydata-i)->Clsprc;        
            	samll_average/=small_period;
		for(int i=0;i<big_period;i++)
			big_average+=(pmydata-i)->Clsprc;
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
		for(int i=0;i<small_period;i++)          
			samll_average+=(pmydata-i)->Clsprc;
        	samll_average/=small_period;
		for(int i=0;i<big_period;i++)       
			big_average+=(pmydata-i)->Clsprc;
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
