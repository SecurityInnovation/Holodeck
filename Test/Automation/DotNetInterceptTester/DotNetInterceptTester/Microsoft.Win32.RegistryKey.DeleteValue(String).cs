using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class DeleteValue_Microsoft_Win32_RegistryKey_System_String
{
public static bool _DeleteValue_Microsoft_Win32_RegistryKey_System_String( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters
   System.String name = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_Microsoft_Win32_RegistryKey.DeleteValue(name);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_Microsoft_Win32_RegistryKey.DeleteValue(name);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
