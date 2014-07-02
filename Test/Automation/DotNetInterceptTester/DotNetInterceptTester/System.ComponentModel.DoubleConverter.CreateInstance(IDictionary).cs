using System;

namespace DotNetInterceptTester.My_System.ComponentModel.DoubleConverter
{
public class CreateInstance_System_ComponentModel_DoubleConverter_System_Collections_IDictionary
{
public static bool _CreateInstance_System_ComponentModel_DoubleConverter_System_Collections_IDictionary( )
{

   //class object
    System.ComponentModel.DoubleConverter _System_ComponentModel_DoubleConverter = new System.ComponentModel.DoubleConverter();

   //Parameters
   System.Collections.IDictionary propertyValues = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_DoubleConverter.CreateInstance(propertyValues);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_DoubleConverter.CreateInstance(propertyValues);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
