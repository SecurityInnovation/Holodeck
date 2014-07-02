namespace DotNetInterceptTester.My_System.ComponentModel.Int64Converter
{
public class IsValid_System_ComponentModel_Int64Converter_System_Object
{
public static bool _IsValid_System_ComponentModel_Int64Converter_System_Object( )
{
   //Parameters
   System.Object _value = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.Int64Converter.IsValid(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.Int64Converter.IsValid(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
