

#pragma once

#include "ADOConn.h"

struct report_var{
	int profit_times,loss_times;
	unsigned long long total_assets,profit,loss;
	int icount;

};

extern HWND hedit1,hedit2,hedit3;


unsigned int __stdcall WorkThrd(void * pParam);
void initial_report_var();
void report();
void append_text(HWND h,char *pStr);