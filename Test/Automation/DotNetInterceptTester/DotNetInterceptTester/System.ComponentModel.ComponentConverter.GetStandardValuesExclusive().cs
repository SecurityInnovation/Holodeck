using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentConverter
{
public class GetStandardValuesExclusive_System_ComponentModel_ComponentConverter
{
public static bool _GetStandardValuesExclusive_System_ComponentModel_ComponentConverter( )
{

   //class object
    System.ComponentModel.ComponentConverter _System_ComponentModel_ComponentConverter = new System.ComponentModel.ComponentConverter();

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
      returnVal_Real = _System_ComponentModel_ComponentConverter.GetStandardValuesExclusive();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ComponentConverter.GetStandardValuesExclusive();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
