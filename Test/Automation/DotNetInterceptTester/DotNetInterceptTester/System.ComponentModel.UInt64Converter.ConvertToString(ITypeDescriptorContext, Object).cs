namespace DotNetInterceptTester.My_System.ComponentModel.UInt64Converter
{
public class ConvertToString_System_ComponentModel_UInt64Converter_System_ComponentModel_ITypeDescriptorContext_System_Object
{
public static bool _ConvertToString_System_ComponentModel_UInt64Converter_System_ComponentModel_ITypeDescriptorContext_System_Object( )
{
   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
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
      returnValue_Real = System.ComponentModel.UInt64Converter.ConvertToString(context,_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.UInt64Converter.ConvertToString(context,_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
