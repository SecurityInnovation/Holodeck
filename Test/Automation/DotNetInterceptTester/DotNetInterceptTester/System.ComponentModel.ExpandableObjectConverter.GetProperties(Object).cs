using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ExpandableObjectConverter
{
public class GetProperties_System_ComponentModel_ExpandableObjectConverter_System_Object
{
public static bool _GetProperties_System_ComponentModel_ExpandableObjectConverter_System_Object( )
{

   //class object
    System.ComponentModel.ExpandableObjectConverter _System_ComponentModel_ExpandableObjectConverter = new System.ComponentModel.ExpandableObjectConverter();

   //Parameters
   System.Object _value = null;

   //ReturnType/Value
   System.ComponentModel.PropertyDescriptorCollection returnVal_Real = null;
   System.ComponentModel.PropertyDescriptorCollection returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_ExpandableObjectConverter.GetProperties(_value);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_ExpandableObjectConverter.GetProperties(_value);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
