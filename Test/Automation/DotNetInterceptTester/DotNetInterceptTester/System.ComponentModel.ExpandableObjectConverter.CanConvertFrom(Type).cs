using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ExpandableObjectConverter
{
public class CanConvertFrom_System_ComponentModel_ExpandableObjectConverter_System_Type
{
public static bool _CanConvertFrom_System_ComponentModel_ExpandableObjectConverter_System_Type( )
{

   //class object
    System.ComponentModel.ExpandableObjectConverter _System_ComponentModel_ExpandableObjectConverter = new System.ComponentModel.ExpandableObjectConverter();

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
      returnVal_Real = _System_ComponentModel_ExpandableObjectConverter.CanConvertFrom(sourceType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ExpandableObjectConverter.CanConvertFrom(sourceType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
