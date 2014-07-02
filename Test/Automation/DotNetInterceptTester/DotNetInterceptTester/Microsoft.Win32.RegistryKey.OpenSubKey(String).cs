using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class OpenSubKey_Microsoft_Win32_RegistryKey_System_String
{
public static bool _OpenSubKey_Microsoft_Win32_RegistryKey_System_String( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters
   System.String name = null;

   //ReturnType/Value
   Microsoft.Win32.RegistryKey returnVal_Real = null;
   Microsoft.Win32.RegistryKey returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _Microsoft_Win32_RegistryKey.OpenSubKey(name);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _Microsoft_Win32_RegistryKey.OpenSubKey(name);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
