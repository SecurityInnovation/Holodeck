using System;

namespace DotNetInterceptTester.My_System.ComponentModel.DateTimeConverter
{
public class GetPropertiesSupported_System_ComponentModel_DateTimeConverter
{
public static bool _GetPropertiesSupported_System_ComponentModel_DateTimeConverter( )
{

   //class object
    System.ComponentModel.DateTimeConverter _System_ComponentModel_DateTimeConverter = new System.ComponentModel.DateTimeConverter();

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
      returnVal_Real = _System_ComponentModel_DateTimeConverter.GetPropertiesSupported();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_DateTimeConverter.GetPropertiesSupported();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
