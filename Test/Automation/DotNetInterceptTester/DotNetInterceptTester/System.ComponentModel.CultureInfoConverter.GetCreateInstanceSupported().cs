using System;

namespace DotNetInterceptTester.My_System.ComponentModel.CultureInfoConverter
{
public class GetCreateInstanceSupported_System_ComponentModel_CultureInfoConverter
{
public static bool _GetCreateInstanceSupported_System_ComponentModel_CultureInfoConverter( )
{

   //class object
    System.ComponentModel.CultureInfoConverter _System_ComponentModel_CultureInfoConverter = new System.ComponentModel.CultureInfoConverter();

   //Parameters

   //ReturnType/Value
   System.Boolean returnVal_Real = false;
   System.Boolean returnVal_Intercepted = false;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_CultureInfoConverter.GetCreateInstanceSupported();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_CultureInfoConverter.GetCreateInstanceSupported();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
