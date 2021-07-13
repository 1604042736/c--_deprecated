#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)

/*����ȼ�*/
enum ErrorLevel
{
	LEVEL_ERROR,	//����
	LEVEL_WARNING	//����
};

/*����״̬*/
enum WorkStage
{
	STAGE_COMPILE,	//����
	STAGE_LINK	//����
};

void warning(char* fmt, char* filename, int line_num, int h, ...);
void error(char* fmt, char* filename, int line_num, int h, ...);