// This is the main DLL file.

#include "stdafx.h"

#include "ProcessTools.h"

namespace StressTester
{    
    System::Int32 ProcessTools::GetChildProcess( System::Int32 parentProcessID )
    {
        PROCESSENTRY32 processEntry;
        HANDLE hSnapshot = NULL;
        
        // Initialize the size...
        processEntry.dwSize = sizeof( processEntry );

        hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS,
                                              0 );

        // If the handle is invalid, we throw an exception...
        if ( hSnapshot == INVALID_HANDLE_VALUE )
        {            
            String __gc* ErrorMessage = __gc new String( "Failed to get snapshot! ERROR: " );
            ErrorMessage->Concat( Convert::ToString( (System::Int32)GetLastError( ) ) );

            throw new System::Exception( ErrorMessage );
        }

        // Now, we start with the first process...
        if ( Process32First( hSnapshot, &processEntry ) )
        {
            if ( processEntry.th32ParentProcessID == (DWORD)parentProcessID )
                return (System::Int32)processEntry.th32ProcessID;
        }
        
        // Continue going down the list of processes until we find the one we need...
        while ( Process32Next( hSnapshot, &processEntry ) )
        {
            if ( processEntry.th32ParentProcessID == (DWORD)parentProcessID )
                return (System::Int32)processEntry.th32ProcessID;
        }

        throw new System::Exception( __gc new String ( "Failed to find the child of the parent process!" ) );
    }
}