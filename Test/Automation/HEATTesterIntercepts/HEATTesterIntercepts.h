#include <windows.h>

// FROM NTDDK
typedef LONG NTSTATUS;
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
    PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;
typedef CONST OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;

typedef NTSTATUS (__stdcall *ZwOpenFilePtr)(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess,
    IN ULONG OpenOptions
    );

// Define NtOpenFile...
typedef void (__stdcall *NtOpenFilePtr)(
  PHANDLE FileHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  PIO_STATUS_BLOCK IoStatusBlock,
  ULONG ShareAccess,
  ULONG OpenOptions );

// Define function pointers...
NtOpenFilePtr NtOpenFile = NULL;
ZwOpenFilePtr ZwOpenFile = NULL;

__declspec(dllexport) HANDLE My_CreateFileA( LPCTSTR lpFileName,
								 DWORD dwDesiredAccess,
								 DWORD dwShareMode,
								 LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								 DWORD dwCreationDisposition,
								 DWORD dwFlagsAndAttributes,
								 HANDLE hTemplateFile );

__declspec(dllexport) BOOL My_WriteFile( HANDLE hFile,
									   LPCVOID lpBuffer,
									   DWORD nNumberOfBytesToWrite,
									   LPDWORD lpNumberOfBytesWritten,
									   LPOVERLAPPED lpOverlapped );

__declspec(dllexport) LPVOID My_TlsGetValue( DWORD dwTlsIndex );
__declspec(dllexport) BOOL My_TlsSetValue( DWORD dwTlsIndex, LPVOID lpTlsValue );
__declspec(dllexport) DWORD My_GetLastError( );
__declspec(dllexport) VOID My_SetLastError( DWORD dwErrCode );

__declspec(dllexport) NTSTATUS __stdcall My_ZwOpenFile(
	 OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  ShareAccess,
    IN ULONG  OpenOptions );

__declspec(dllexport) void __stdcall My_NtOpenFile(
  PHANDLE FileHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  PIO_STATUS_BLOCK IoStatusBlock,
  ULONG ShareAccess,
  ULONG OpenOptions
);