using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TimeSpanConverter
{
public class GetStandardValuesSupported_System_ComponentModel_TimeSpanConverter
{
public static bool _GetStandardValuesSupported_System_ComponentModel_TimeSpanConverter( )
{

   //class object
    System.ComponentModel.TimeSpanConverter _System_ComponentModel_TimeSpanConverter = new System.ComponentModel.TimeSpanConverter();

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
      returnVal_Real = _System_ComponentModel_TimeSpanConverter.GetStandardValuesSupported();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_TimeSpanConverter.GetStandardValuesSupported();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
