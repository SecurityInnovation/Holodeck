// ProcessTools.h

#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

using namespace System;

namespace StressTester
{
	public __gc class ProcessTools
	{
        public:
            System::Int32 GetChildProcess( System::Int32 parentProcessID );
	};
}
