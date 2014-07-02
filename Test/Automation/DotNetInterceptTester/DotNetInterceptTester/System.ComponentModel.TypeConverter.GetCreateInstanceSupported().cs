using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeConverter
{
public class GetCreateInstanceSupported_System_ComponentModel_TypeConverter
{
public static bool _GetCreateInstanceSupported_System_ComponentModel_TypeConverter( )
{

   //class object
    System.ComponentModel.TypeConverter _System_ComponentModel_TypeConverter = new System.ComponentModel.TypeConverter();

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
      returnVal_Real = _System_ComponentModel_TypeConverter.GetCreateInstanceSupported();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_TypeConverter.GetCreateInstanceSupported();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
