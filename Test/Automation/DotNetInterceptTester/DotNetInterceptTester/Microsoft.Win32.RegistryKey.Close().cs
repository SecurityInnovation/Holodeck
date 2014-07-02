using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class Close_Microsoft_Win32_RegistryKey
{
public static bool _Close_Microsoft_Win32_RegistryKey( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _Microsoft_Win32_RegistryKey.Close();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _Microsoft_Win32_RegistryKey.Close();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
