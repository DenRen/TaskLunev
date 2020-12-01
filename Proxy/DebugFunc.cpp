#define _CRT_SECURE_NO_WARNINGS

#include "DebugFunc.h"
#include <cstdio>
#include <stdexcept>
#include <cstring>

void NOP ()
{}

void PrintLogInfo (const char file[], int line, bool inFile, bool end)
{
	static bool AlreadyPrinted = false;

	const int sizeLog = 512;
	char log[sizeLog] = "";

	if (!end)
	{
		if (!AlreadyPrinted)
		{
			sprintf (log, R"(/////////////////////____________DUMP ERROR____________\\\\\\\\\\\\\\\\\\\\\)");
		}
		sprintf (log, "%s\n\n"
			"FILE: %s\n"
			"LINE: %d\n", log, file, line);

		if (AlreadyPrinted)
		{
			sprintf (log, "%s\n", log);
		}
		else
		{
			AlreadyPrinted = true;
			sprintf (log, "%s\nerror:\n%s\n\n", log, strerror (errno));
		}
	}
	else
	{
		if (AlreadyPrinted)
		{
			sprintf (log, R"(\\\\\\\\\\\\\\\\\\\\\_______________END________________/////////////////////)");
		}
	}

	FILE *outputStream = nullptr;

	if (inFile)
	{
		const char logFileName[] = "log.txt";
		outputStream = fopen (logFileName, "a");
		if (outputStream == nullptr)
		{
			snprintf (log, sizeLog, "Failed to create log file!\n%s", log);

			return;
		}
	}
	else
	{
		outputStream = stdout;
	}

	fprintf (outputStream, "%s", log);
	fflush (outputStream);

	if (inFile)
	{
		fclose (outputStream);
	}
}

void DebugEndMain ()
{
	END_DUMP_DEBUG_INFO;
}

#undef _CRT_SECURE_NO_WARNINGS