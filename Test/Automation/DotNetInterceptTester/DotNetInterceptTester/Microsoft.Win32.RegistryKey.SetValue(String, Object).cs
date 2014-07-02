using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class SetValue_Microsoft_Win32_RegistryKey_System_String_System_Object
{
public static bool _SetValue_Microsoft_Win32_RegistryKey_System_String_System_Object( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters
   System.String name = null;
   System.Object _value = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_Microsoft_Win32_RegistryKey.SetValue(name,_value);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_Microsoft_Win32_RegistryKey.SetValue(name,_value);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
