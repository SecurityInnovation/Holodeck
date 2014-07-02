using System;
using System.Runtime.InteropServices;

namespace StressTester
{
    /*
        typedef struct tagPROCESSENTRY32 
        {
            DWORD dwSize;
            DWORD cntUsage;
            DWORD th32ProcessID;
            ULONG_PTR th32DefaultHeapID;
            DWORD th32ModuleID;
            DWORD cntThreads;
            DWORD th32ParentProcessID;
            LONG pcPriClassBase;
            DWORD dwFlags;
            TCHAR szExeFile[MAX_PATH];
        } PROCESSENTRY32, *PPROCESSENTRY32;
        */

    // Set this struct up to be a sequential C++/C style struct when marshalled over.
    [StructLayout(LayoutKind.Explicit)]
    public struct PROCESSENTRY32
    {
        [FieldOffset(0)] public Int32 dwSize;
        [FieldOffset(1)] public Int32 cntUsage;
        [FieldOffset(2)] public Int32 th32ProcessID;
        //public UIntPtr th32DefaultHeapID;
        //public Int32 th32ModuleID;
        [FieldOffset(5)] public Int32 cntThreads;
        [FieldOffset(6)] public Int32 th32ParentProcessID;
        //public long pcPriClassBase;
        //public Int32 dwFlags;
        //public char [] szExeFile;
    }

	/// <summary>
	/// Summary description for ProcessTools.
	/// </summary>
	public class ProcessTools
	{      
        // Define the snap process flag...
        public static Int32 TH32CS_SNAPPROCESS = 0x00000002;

        [DllImportAttribute("KERNEL32.DLL")]
        public static extern IntPtr CreateToolhelp32Snapshot( Int32 dwFlags, Int32 th32ProcessID );

        [DllImportAttribute("KERNEL32.DLL")]
        public static extern bool Process32First( IntPtr hSnapshot, ref PROCESSENTRY32 processEntry32 );

        [DllImportAttribute("KERNEL32.DLL")]
        public static extern bool Process32Next( IntPtr hSnapshot, ref PROCESSENTRY32 processEntry32 );


        /// <summary>
        /// Searches through processes on the system to determine the child process of the specified
        /// process ID...
        /// </summary>
        /// <param name="processID"></param>
        public static Int32 GetChildProcess( Int32 processID )
        {
            PROCESSENTRY32 processEntry = new PROCESSENTRY32( );
            processEntry.dwSize = 296; // Size of the PROCESSENTRY32 struct...
            //processEntry.th32DefaultHeapID = new UIntPtr( 0 );
            //processEntry.szExeFile = new char[260];

            // Get a handle to the snapshot...
            IntPtr hSnapshotHandle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

            // Get the first Process...
            if ( Process32First( hSnapshotHandle, ref processEntry ) )
            {
                if ( processEntry.th32ParentProcessID == processID )
                {
                    return processEntry.th32ProcessID;
                }
            }
            
            while ( Process32Next( hSnapshotHandle, ref processEntry ) )
            {
                if ( processEntry.th32ParentProcessID == processID )
                {
                    return processEntry.th32ProcessID;
                }
            }

            return 0;
        }		
	}
}
