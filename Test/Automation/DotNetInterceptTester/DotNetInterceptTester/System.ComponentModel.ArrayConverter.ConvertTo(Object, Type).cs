using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ArrayConverter
{
public class ConvertTo_System_ComponentModel_ArrayConverter_System_Object_System_Type
{
public static bool _ConvertTo_System_ComponentModel_ArrayConverter_System_Object_System_Type( )
{

   //class object
    System.ComponentModel.ArrayConverter _System_ComponentModel_ArrayConverter = new System.ComponentModel.ArrayConverter();

   //Parameters
   System.Object _value = null;
   System.Type destinationType = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ArrayConverter.ConvertTo(_value,destinationType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ArrayConverter.ConvertTo(_value,destinationType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
