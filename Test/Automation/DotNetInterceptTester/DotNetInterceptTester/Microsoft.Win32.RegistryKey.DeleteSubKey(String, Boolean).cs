using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class DeleteSubKey_Microsoft_Win32_RegistryKey_System_String_System_Boolean
{
public static bool _DeleteSubKey_Microsoft_Win32_RegistryKey_System_String_System_Boolean( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters
   System.String subkey = null;
   System.Boolean throwOnMissingSubKey = false;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_Microsoft_Win32_RegistryKey.DeleteSubKey(subkey,throwOnMissingSubKey);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_Microsoft_Win32_RegistryKey.DeleteSubKey(subkey,throwOnMissingSubKey);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
