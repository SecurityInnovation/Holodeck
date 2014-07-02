namespace DotNetInterceptTester.My_System.ComponentModel.StringConverter
{
public class CanConvertFrom_System_ComponentModel_StringConverter_System_ComponentModel_ITypeDescriptorContext_System_Type
{
public static bool _CanConvertFrom_System_ComponentModel_StringConverter_System_ComponentModel_ITypeDescriptorContext_System_Type( )
{
   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
   System.Type sourceType = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.StringConverter.CanConvertFrom(context,sourceType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.StringConverter.CanConvertFrom(context,sourceType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
