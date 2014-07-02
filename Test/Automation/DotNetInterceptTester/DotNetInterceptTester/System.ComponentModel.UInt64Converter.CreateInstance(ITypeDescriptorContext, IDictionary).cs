namespace DotNetInterceptTester.My_System.ComponentModel.UInt64Converter
{
public class CreateInstance_System_ComponentModel_UInt64Converter_System_ComponentModel_ITypeDescriptorContext_System_Collections_IDictionary
{
public static bool _CreateInstance_System_ComponentModel_UInt64Converter_System_ComponentModel_ITypeDescriptorContext_System_Collections_IDictionary( )
{
   //Parameters
   System.ComponentModel.ITypeDescriptorContext context = null;
   System.Collections.IDictionary propertyValues = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.UInt64Converter.CreateInstance(context,propertyValues);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.UInt64Converter.CreateInstance(context,propertyValues);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
