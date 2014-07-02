namespace DotNetInterceptTester.My_System.ComponentModel.Int64Converter
{
public class ConvertToString_System_ComponentModel_Int64Converter_System_ComponentModel_ITypeDescriptorContext_System_Globalization_CultureInfo_System_Object
{
public static bool _ConvertToString_System_ComponentModel_Int64Converter_System_ComponentModel_ITypeDescriptorContext_System_Globalization_CultureInfo_System_Object( )
{
   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
   System.Globalization.CultureInfo culture = null;
   System.Object _value = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.Int64Converter.ConvertToString(context,culture,_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.Int64Converter.ConvertToString(context,culture,_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
