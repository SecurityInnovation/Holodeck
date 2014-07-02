using System;

namespace DotNetInterceptTester.My_System.ComponentModel.CultureInfoConverter
{
public class IsValid_System_ComponentModel_CultureInfoConverter_System_ComponentModel_ITypeDescriptorContext_System_Object
{
public static bool _IsValid_System_ComponentModel_CultureInfoConverter_System_ComponentModel_ITypeDescriptorContext_System_Object( )
{

   //class object
    System.ComponentModel.CultureInfoConverter _System_ComponentModel_CultureInfoConverter = new System.ComponentModel.CultureInfoConverter();

   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
   System.Object _value = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = false;
   System.Boolean returnVal_Intercepted = false;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_CultureInfoConverter.IsValid(context,_value);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_CultureInfoConverter.IsValid(context,_value);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
