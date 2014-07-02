using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeConverter
{
public class GetProperties_System_ComponentModel_TypeConverter_System_ComponentModel_ITypeDescriptorContext_System_Object
{
public static bool _GetProperties_System_ComponentModel_TypeConverter_System_ComponentModel_ITypeDescriptorContext_System_Object( )
{

   //class object
    System.ComponentModel.TypeConverter _System_ComponentModel_TypeConverter = new System.ComponentModel.TypeConverter();

   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
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
      returnVal_Real = _System_ComponentModel_TypeConverter.GetProperties(context,_value);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_TypeConverter.GetProperties(context,_value);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
