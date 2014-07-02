using System;

namespace DotNetInterceptTester.My_Microsoft.Win32.RegistryKey
{
public class CreateObjRef_Microsoft_Win32_RegistryKey_System_Type
{
public static bool _CreateObjRef_Microsoft_Win32_RegistryKey_System_Type( )
{

   //class object
    Microsoft.Win32.RegistryKey _Microsoft_Win32_RegistryKey = new Microsoft.Win32.RegistryKey();

   //Parameters
   System.Type requestedType = null;

   //ReturnType/Value
   System.Runtime.Remoting.ObjRef returnVal_Real = null;
   System.Runtime.Remoting.ObjRef returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _Microsoft_Win32_RegistryKey.CreateObjRef(requestedType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _Microsoft_Win32_RegistryKey.CreateObjRef(requestedType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
