using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentConverter
{
public class CanConvertFrom_System_ComponentModel_ComponentConverter_System_Type
{
public static bool _CanConvertFrom_System_ComponentModel_ComponentConverter_System_Type( )
{

   //class object
    System.ComponentModel.ComponentConverter _System_ComponentModel_ComponentConverter = new System.ComponentModel.ComponentConverter();

   //Parameters
   System.Type sourceType = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = false;
   System.Boolean returnVal_Intercepted = false;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ComponentConverter.CanConvertFrom(sourceType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ComponentConverter.CanConvertFrom(sourceType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
