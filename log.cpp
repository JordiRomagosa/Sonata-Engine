#ifndef __Log_CPP__
#define __Log_CPP__

#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	if (App != nullptr && App->editor != nullptr)
		App->editor->consoleBuffer.append(tmp_string2);
}

#endif __Log_CPP__