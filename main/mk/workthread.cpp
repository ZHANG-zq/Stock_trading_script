#include "StdAfx.h"    //必须
#include "ADOConn.h"
#include <string>
#include <fstream>
#include "workthread.h"
#include "Turtle.h"

using namespace std;

#define filepath "" 

/*全局变量尽量少用，毕竟不好维护，且占内存*/
HWND hedit1=NULL,hedit2=NULL,hedit3=NULL;         //作为外部变量，对整个解决方案可见

char **pstr_code=NULL;
int pstr_code_size=0;    //为了方便删除动态数组,真想不到怎样获取动态二维数组大小
char pstr_start_time[11]={' '};  //初值为一个空格字符，不是空字符串
char pstr_end_time[11]={' '};

report_var *preport_var=NULL,*preport_var_old=NULL;          


void write_in_db(ADOConn *pa);
void calculate(ADOConn *pa);


unsigned int __stdcall WorkThrd(void * pParam)  //参数是对话框窗口类
{


	ADOConn a;
	a.OnInitADOConn();

	//write_in_db(&a);
	calculate(&a);
	

	AfxMessageBox("工作完成");
    return 0;
}

/*
void write_in_db(ADOConn *pa)
{
	char tstr[18][17]={'0'};                 //无所谓速度。速度最快的是一次性读取数据，然后写入。
	string s;   //string有重载“+”,连接多个字符串时方便
	_bstr_t bstrSQL;

	int icount=0;



	FILE* pfile;
	pfile=fopen(filepath,"r");
    //不知为什么，总会多读一行，所以减去1
    while(fscanf(pfile,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",&tstr[1],&tstr[2],&tstr[3],&tstr[4],&tstr[5],&tstr[6],&tstr[7],&tstr[8],&tstr[9],&tstr[10],&tstr[11],&tstr[12],&tstr[13],&tstr[14],&tstr[15],&tstr[16],&tstr[17])==17)     //返回值为成功传递的参数个数      %ld不行，要%lld才行
	{

	//s=s+tstr[1]+"'"+tstr[1]+"','"+tstr[2]+"','"+tstr[3]+"','"+tstr[4]+"','"+tstr[5]+"','"+tstr[6]+"','"+tstr[7]+"','"+tstr[8]+"','"+tstr[9]+"','"+tstr[10]+"','"+tstr[11]+"','"+tstr[12]+"','"+tstr[13]+"','"+tstr[14]+"','"+tstr[15]+"','"+tstr[16]+"','"+tstr[17]+"')";
	//AfxMessageBox(s.c_str());


	//新的证券代码，建立个新表,如果表已经存在，依然能够运行下去。
	if(strcmp(tstr[0],tstr[1])!=0)   //字符数组的操作要用函数来，st1=st2相等则输出0，st1<st2输出-1，st1>st2输出1
	{
	bstrSQL="CREATE TABLE [";         //以纯数组做表名，加个中括号就好了，不知为什么
	bstrSQL+=tstr[1];
	bstrSQL+="](Stkcd varchar(7),Trddt date,Opnprc decimal(7,3),Hiprc decimal(7,3),Loprc decimal(7,3),Clsprc decimal(7,3),Dnshrtrd numeric(12, 0),Dnvaltrd money,Dsmvosd money,Dsmvtll money,Dretwd varchar(12),Dretnd varchar(12),Adjprcwd decimal(10,3),Adjprcnd decimal(10,3),Markettype tinyint,Capchgdt date,Trdsta tinyint)";
	//AfxMessageBox(bstrSQL);
	pa->ExecuteSQL(bstrSQL);
	memcpy(tstr[0],tstr[1],6);   //copy前6个字符
	}

	
	s="INSERT INTO [";
	s=s+tstr[1]+"] VALUES ('"+tstr[1]+"','"+tstr[2]+"','"+tstr[3]+"','"+tstr[4]+"','"+tstr[5]+"','"+tstr[6]+"','"+tstr[7]+"','"+tstr[8]+"','"+tstr[9]+"','"+tstr[10]+"','"+tstr[11]+"','"+tstr[12]+"','"+tstr[13]+"','"+tstr[14]+"','"+tstr[15]+"','"+tstr[16]+"','"+tstr[17]+"')";
	//AfxMessageBox(s.c_str());
	bstrSQL=s.c_str();
	pa->ExecuteSQL(bstrSQL);

	}

	icount++;
}

*/

void calculate(ADOConn *pa)
{
	_RecordsetPtr m_pRecordset;  //证券代码
	_RecordsetPtr m_pRecordset2;   //日交易数据
	_bstr_t bstrSQL;

	if(pstr_code==NULL) //没有设置品种范围,就默认设置为所有品种
	{
		bstrSQL="select code from code";
		m_pRecordset=pa->GetRecordSet(bstrSQL);

		pstr_code=new char*[m_pRecordset->RecordCount];        //分配二维数组存放代码
		pstr_code_size=m_pRecordset->RecordCount;              //同时记住二维数组大小

		int i=0;
		for(m_pRecordset->MoveFirst();!m_pRecordset->adoEOF;m_pRecordset->MoveNext(),i++)  //遍历所有的证券代码
		{
				pstr_code[i]=new char[6]; 
				strcpy(pstr_code[i],(_bstr_t)m_pRecordset->GetCollect("code"));
				//AfxMessageBox(pstr_code[i]);
		}
		m_pRecordset->Close();
		m_pRecordset=NULL;
	}


	preport_var=preport_var_old=new report_var[10]; //分配数组用于report输出
	initial_report_var();

	for(int i=0;i<pstr_code_size;i++)    //遍历每一只股票
	{
		bstrSQL="select [Opnprc],[Hiprc],[Loprc],[Clsprc],[Adjprcnd],[Trdsta] from [";         //这里选择要用到的字段。 这里我只关注价格。
		bstrSQL+=pstr_code[i];
		bstrSQL+="]";      //别忘了中括号

		if(pstr_start_time[0]!=' ' || pstr_end_time[0]!=' ')   //没有设置时间范围就跳过（0与空格的ansi编码不同。）
		{
			bstrSQL+=" WHERE ";
			if(pstr_start_time[0]!=' ' && pstr_end_time[0]!=' ')
			{
				bstrSQL+="Trddt>='";
				bstrSQL+=pstr_start_time;
				bstrSQL+="' AND Trddt<='";
				bstrSQL+=pstr_end_time;
				bstrSQL+="'";
				goto s; //跳过后面
			}

			if(pstr_start_time[0]!=' ')
			{
				bstrSQL+="Trddt>='";
				bstrSQL+=pstr_start_time;
				bstrSQL+="'";
			}

			if(pstr_end_time[0]!=' ')
			{
				bstrSQL+="Trddt<='";
				bstrSQL+=pstr_end_time;
				bstrSQL+="'";
			}

		}

		s:
		//AfxMessageBox(bstrSQL); 
		m_pRecordset2=pa->GetRecordSet(bstrSQL);  
		
		long num=m_pRecordset2->RecordCount;

		if(num!=0)  //没有选择股票还分配个屁内存啊？
		{
				pmydata_old=new data[num];    //如果RecordCount为-1就不行; pmydata_old一直指向内存地址不变
				pmydata=pmydata_old;
						//recordset不适合分析数据，先转到内存块里，然后就是分析。
				double Clsprc,percent;
				for(m_pRecordset2->MoveFirst();!m_pRecordset2->adoEOF;m_pRecordset2->MoveNext(),pmydata++)
				{
					pmydata->Clsprc =atof((_bstr_t)m_pRecordset2->GetCollect("Adjprcnd")); 	
					/*价格调整*/
					Clsprc=atof((_bstr_t)m_pRecordset2->GetCollect("Clsprc"));
					percent=pmydata->Clsprc/Clsprc;

					pmydata->Opnprc=atof((_bstr_t)m_pRecordset2->GetCollect("Opnprc"))*percent;
					pmydata->Hiprc=atof((_bstr_t)m_pRecordset2->GetCollect("Opnprc"))*percent;
					pmydata->Loprc=atof((_bstr_t)m_pRecordset2->GetCollect("Loprc"))*percent;
					pmydata->Trdsta=atoi((_bstr_t)m_pRecordset2->GetCollect("Trdsta"));
				}

		}

		m_pRecordset2->Close();   //用完数据集后立即销毁
		m_pRecordset2=NULL;

		pmydata=pmydata_old;

		SetWindowText(hedit3," ");
		/*数据准备完后，模拟市场运行,（多个条件）*/
		for(int l=0;l<10;l++)
		{
			  Turtle *tt=new Turtle;
		      tt->run_market(pstr_code[i],num,(l+1)*5,(preport_var+l));
		      delete tt;
		}

		if(pmydata_old!=NULL)     //先检查后释放。
		delete [] pmydata_old;    //记得清楚，不然会内存泄露
		pmydata=pmydata_old=NULL;
	}

	report();  //report in edit3


	delete [] preport_var_old;        //由于preport_var已经指向到最后那个元素，所以不能直接delete [] preport_var;
	preport_var=preport_var_old=NULL;
	pa->ExitConnect();       //与数据库连接断开

	//pstr_code的内存空间不用释放，留待下次用
}



		/*
		_bstr_t bstrSQL="select code from code";
		m_pRecordset=pa->GetRecordSet(bstrSQL);
		preport_var=new report_var[m_pRecordset->RecordCount];
		for(m_pRecordset->MoveFirst();!m_pRecordset->adoEOF;m_pRecordset->MoveNext())  //遍历所有的证券代码
		{

		bstrSQL="select [Adjprcnd] from [";         //这里选择要用到的字段。 这里我只用一个“不考虑现金红利的收盘价的可比价格”
		bstrSQL+=(_bstr_t)m_pRecordset->GetCollect("code");
		bstrSQL+="]";      //别忘了中括号
		//AfxMessageBox(bstrSQL);
		m_pRecordset2=pa->GetRecordSet(bstrSQL);   



		long num=m_pRecordset2->RecordCount;
		pmydata_old=new data[num];    //如果RecordCount为-1就不行
		pmydata=pmydata_old;

						//recordset不适合分析数据，先转到内存块里，然后就是分析。
		for(m_pRecordset2->MoveFirst();!m_pRecordset2->adoEOF;m_pRecordset2->MoveNext(),pmydata++)
		{
					bstrSQL=(_bstr_t)m_pRecordset2->GetCollect("Adjprcnd");
					pmydata->Clsprc = atof(bstrSQL); 

		}

		m_pRecordset2->Close();
		m_pRecordset2=NULL;

		pmydata=pmydata_old;

		Moving_Average ma;
		ma.run_market(num,(char *)(_bstr_t)m_pRecordset->GetCollect("code"));


		delete [] pmydata_old;    //记得清楚，不然会内存泄露
		pmydata=pmydata_old=NULL;

		}
	
		delete [] preport_var;
		m_pRecordset->Close();
		m_pRecordset=NULL;
		pa->ExitConnect();
	}
	*/



void initial_report_var()
{
	for(int l=0;l<10;l++)
	{
		(preport_var+l)->icount=0; 
		(preport_var+l)->total_assets=0;      
		(preport_var+l)->profit_times=0;
		(preport_var+l)->loss_times=0;
		(preport_var+l)->profit=0;
		(preport_var+l)->loss=0;
	}
}

void report()
{
	CString strtemp;
	SetWindowText(hedit3," ");
		for(int l=0;l<10;l++)
	{
		if((preport_var+l)->icount==0)
		{  
			strtemp.Format("计算0只股票\n\n周期%d天",(preport_var+l)->icount,(l+1)*5);
			append_text(hedit1,(LPTSTR)(LPCTSTR)strtemp);
			continue;
		}

		int aver_profit_times=0,aver_loss_times=0;
	    unsigned long long aver_total_assets=0,aver_profit=0,aver_loss=0;

		aver_loss_times=(preport_var+l)->loss_times/(preport_var+l)->icount;      //由于这里等价于aver_loss_times=aver_loss_times+(preport_var-i)->loss_times;所以必须先赋初值
		aver_profit_times=(preport_var+l)->profit_times/(preport_var+l)->icount;            
		aver_total_assets=(preport_var+l)->total_assets/(preport_var+l)->icount; 
		aver_profit=(preport_var+l)->profit/(preport_var+l)->icount; 
		aver_loss=(preport_var+l)->loss/(preport_var+l)->icount; 


		strtemp.Format("计算%d只股票\n\n周期%d天\n\n平均总资产余额%I64u\n\n平均盈利次数%d\n\n平均亏损次数%d\n\n平均盈利总额%I64u\n\n平均亏损总额%I64u",(preport_var+l)->icount,(l+1)*5,aver_total_assets,aver_profit_times,aver_loss_times,aver_profit,aver_loss);
	    append_text(hedit3,(LPTSTR)(LPCTSTR)strtemp);
	}

}



void append_text(HWND h,char *pStr)
{
	int nLength =SendMessage(h, WM_GETTEXTLENGTH,0,0); 
	SendMessage(h, EM_SETSEL, nLength, nLength);    //虽然postmessage更快，但postmessage是异步，要保证参数值的不变。
    //SendMessage(hEdit, EM_SCROLLCARET, 0, 0);
    SendMessage(h, EM_REPLACESEL, TRUE, (LPARAM)pStr);
}