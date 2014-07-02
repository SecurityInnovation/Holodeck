

#include "LogReaderV2_5.h"

#using <mscorlib.dll>
using namespace System;
using namespace HoloScript;

// Main entrypoint...
int main( )
{
	LogReaderV2_5 * logReader = new LogReaderV2_5( );

	// Open the log file...
//	logReader->OpenLogFile( "C:\\Program Files\\Security Innovation\\Holodeck Enterprise Edition\\logs\\424.hdl" );

	logReader->OpenLogFile( "C:\\Program Files\\Security Innovation\\Holodeck Enterprise Edition\\logs\\1376.hdl", "C:\\test.txt" );

}
