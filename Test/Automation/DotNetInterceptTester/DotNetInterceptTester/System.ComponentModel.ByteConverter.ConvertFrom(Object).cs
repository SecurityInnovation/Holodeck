using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ByteConverter
{
public class ConvertFrom_System_ComponentModel_ByteConverter_System_Object
{
public static bool _ConvertFrom_System_ComponentModel_ByteConverter_System_Object( )
{

   //class object
    System.ComponentModel.ByteConverter _System_ComponentModel_ByteConverter = new System.ComponentModel.ByteConverter();

   //Parameters
   System.Object _value = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ByteConverter.ConvertFrom(_value);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ByteConverter.ConvertFrom(_value);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
