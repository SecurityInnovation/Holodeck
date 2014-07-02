using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentResourceManager
{
public class GetObject_System_ComponentModel_ComponentResourceManager_System_String_System_Globalization_CultureInfo
{
public static bool _GetObject_System_ComponentModel_ComponentResourceManager_System_String_System_Globalization_CultureInfo( )
{

   //class object
    System.ComponentModel.ComponentResourceManager _System_ComponentModel_ComponentResourceManager = new System.ComponentModel.ComponentResourceManager();

   //Parameters
   System.String name = null;
   System.Globalization.CultureInfo culture = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ComponentResourceManager.GetObject(name,culture);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ComponentResourceManager.GetObject(name,culture);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
