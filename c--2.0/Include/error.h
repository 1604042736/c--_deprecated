#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

/*错误等级*/
enum ErrorLevel
{
	LEVEL_ERROR,	//错误
	LEVEL_WARNING	//警告
};

/*工作状态*/
enum WorkStage
{
	STAGE_COMPILE,	//编译
	STAGE_LINK	//链接
};

void warning(char* fmt, char* filename, int line_num, int h, ...);
void error(char* fmt, char* filename, int line_num, int h, ...);