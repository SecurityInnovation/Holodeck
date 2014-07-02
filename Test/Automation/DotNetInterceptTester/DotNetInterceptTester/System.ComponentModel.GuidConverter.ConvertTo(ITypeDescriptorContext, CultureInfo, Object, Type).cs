using System;

namespace DotNetInterceptTester.My_System.ComponentModel.GuidConverter
{
public class ConvertTo_System_ComponentModel_GuidConverter_System_ComponentModel_ITypeDescriptorContext_System_Globalization_CultureInfo_System_Object_System_Type
{
public static bool _ConvertTo_System_ComponentModel_GuidConverter_System_ComponentModel_ITypeDescriptorContext_System_Globalization_CultureInfo_System_Object_System_Type( )
{

   //class object
    System.ComponentModel.GuidConverter _System_ComponentModel_GuidConverter = new System.ComponentModel.GuidConverter();

   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
   System.Globalization.CultureInfo culture = null;
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
      returnVal_Real = _System_ComponentModel_GuidConverter.ConvertTo(context,culture,_value,destinationType);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_GuidConverter.ConvertTo(context,culture,_value,destinationType);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
