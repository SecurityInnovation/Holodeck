using System;

namespace DotNetInterceptTester.My_System.ComponentModel.UInt16Converter
{
public class ConvertFromString_System_ComponentModel_UInt16Converter_System_String
{
public static bool _ConvertFromString_System_ComponentModel_UInt16Converter_System_String( )
{

   //class object
    System.ComponentModel.UInt16Converter _System_ComponentModel_UInt16Converter = new System.ComponentModel.UInt16Converter();

   //Parameters
   System.String text = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_UInt16Converter.ConvertFromString(text);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_UInt16Converter.ConvertFromString(text);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
