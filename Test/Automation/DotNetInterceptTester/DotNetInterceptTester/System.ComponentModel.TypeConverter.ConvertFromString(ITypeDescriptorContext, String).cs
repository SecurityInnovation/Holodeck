using System;

namespace DotNetInterceptTester.My_System.ComponentModel.TypeConverter
{
public class ConvertFromString_System_ComponentModel_TypeConverter_System_ComponentModel_ITypeDescriptorContext_System_String
{
public static bool _ConvertFromString_System_ComponentModel_TypeConverter_System_ComponentModel_ITypeDescriptorContext_System_String( )
{

   //class object
    System.ComponentModel.TypeConverter _System_ComponentModel_TypeConverter = new System.ComponentModel.TypeConverter();

   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
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
      returnVal_Real = _System_ComponentModel_TypeConverter.ConvertFromString(context,text);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_TypeConverter.ConvertFromString(context,text);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
