using System;

namespace DotNetInterceptTester.My_System.ComponentModel.CharConverter
{
public class CanConvertTo_System_ComponentModel_CharConverter_System_Type
{
public static bool _CanConvertTo_System_ComponentModel_CharConverter_System_Type( )
{

   //class object
    System.ComponentModel.CharConverter _System_ComponentModel_CharConverter = new System.ComponentModel.CharConverter();

   //Parameters
   System.Type destinationType = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = false;
   System.Boolean returnVal_Intercepted = false;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_CharConverter.CanConvertTo(destinationType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_CharConverter.CanConvertTo(destinationType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
