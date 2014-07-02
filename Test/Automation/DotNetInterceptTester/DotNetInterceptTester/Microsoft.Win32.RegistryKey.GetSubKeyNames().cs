using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class GetSubKeyNames_Microsoft_Win32_RegistryKey
{
public static bool _GetSubKeyNames_Microsoft_Win32_RegistryKey( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters

   //ReturnType/Value
   System.String[] returnVal_Real = null;
   System.String[] returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _Microsoft_Win32_RegistryKey.GetSubKeyNames();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _Microsoft_Win32_RegistryKey.GetSubKeyNames();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
