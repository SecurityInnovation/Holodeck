using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class OpenRemoteBaseKey_Microsoft_Win32_RegistryHive_System_String
{
public static bool _OpenRemoteBaseKey_Microsoft_Win32_RegistryHive_System_String( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters
   Microsoft.Win32.RegistryHive hKey = null;
   System.String machineName = null;

   //ReturnType/Value
   Microsoft.Win32.RegistryKey returnVal_Real = null;
   Microsoft.Win32.RegistryKey returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _Microsoft_Win32_RegistryKey.OpenRemoteBaseKey(hKey,machineName);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _Microsoft_Win32_RegistryKey.OpenRemoteBaseKey(hKey,machineName);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
