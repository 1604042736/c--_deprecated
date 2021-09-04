#pragma once

#include "globals.h"
#include "compiler.h"

enum e_ErrorLevel
{
	LEVEL_WARNING,
	LEVEL_ERROR,
};

enum e_WorkStage
{
	STAGE_COMPILE,
	STAGE_LINK,
};

void warning(char* fmt,char* filename,int line_num,int h,...);
void error(char* fmt,char* filename,int line_num,int h,...);
