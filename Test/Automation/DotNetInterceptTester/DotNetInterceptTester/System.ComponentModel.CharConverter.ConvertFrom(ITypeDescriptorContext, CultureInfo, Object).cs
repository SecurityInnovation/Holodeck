using System;

namespace DotNetInterceptTester.My_System.ComponentModel.CharConverter
{
public class ConvertFrom_System_ComponentModel_CharConverter_System_ComponentModel_ITypeDescriptorContext_System_Globalization_CultureInfo_System_Object
{
public static bool _ConvertFrom_System_ComponentModel_CharConverter_System_ComponentModel_ITypeDescriptorContext_System_Globalization_CultureInfo_System_Object( )
{

   //class object
    System.ComponentModel.CharConverter _System_ComponentModel_CharConverter = new System.ComponentModel.CharConverter();

   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
   System.Globalization.CultureInfo culture = null;
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
      returnVal_Real = _System_ComponentModel_CharConverter.ConvertFrom(context,culture,_value);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_CharConverter.ConvertFrom(context,culture,_value);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
