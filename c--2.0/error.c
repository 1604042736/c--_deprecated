#include "Include/error.h"

void handle_exception(char* filename, int line_num, int h, int stage, int level, char* fmt, va_list ap)
{
	char buf[1024];
	vsprintf(buf, fmt, ap);
	if (stage == STAGE_COMPILE)
	{
		if (level == LEVEL_WARNING)
		{
			printf("%s:%d:%d:¾¯¸æ:%s\n", filename, line_num, h, buf);
		}
		else
		{
			printf("%s:%d:%d:´íÎó:%s\n", filename, line_num, h, buf);
			exit(-1);
		}
	}
	else
	{
		printf("Á´½Ó´íÎó: %s!\n", buf);
		exit(-1);
	}
}

void warning(char* fmt, char* filename, int line_num, int h, ...)
{
	va_list ap;
	va_start(ap, h);
	handle_exception(filename, line_num, h, STAGE_COMPILE, LEVEL_WARNING, fmt, ap);
	va_end(ap);
}

void error(char* fmt, char* filename, int line_num, int h, ...)
{
	va_list ap;
	va_start(ap, h);
	handle_exception(filename, line_num, h, STAGE_COMPILE, LEVEL_ERROR, fmt, ap);
	va_end(ap);
}