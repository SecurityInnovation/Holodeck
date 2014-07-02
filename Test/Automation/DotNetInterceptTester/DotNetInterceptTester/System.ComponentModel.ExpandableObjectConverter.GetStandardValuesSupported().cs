using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ExpandableObjectConverter
{
public class GetStandardValuesSupported_System_ComponentModel_ExpandableObjectConverter
{
public static bool _GetStandardValuesSupported_System_ComponentModel_ExpandableObjectConverter( )
{

   //class object
    System.ComponentModel.ExpandableObjectConverter _System_ComponentModel_ExpandableObjectConverter = new System.ComponentModel.ExpandableObjectConverter();

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
      returnVal_Real = _System_ComponentModel_ExpandableObjectConverter.GetStandardValuesSupported();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ExpandableObjectConverter.GetStandardValuesSupported();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
