namespace DotNetInterceptTester.My_System.ComponentModel.TimeSpanConverter
{
public class CanConvertFrom_System_ComponentModel_TimeSpanConverter_System_Type
{
public static bool _CanConvertFrom_System_ComponentModel_TimeSpanConverter_System_Type( )
{
   //Parameters
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
      returnValue_Real = System.ComponentModel.TimeSpanConverter.CanConvertFrom(sourceType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.TimeSpanConverter.CanConvertFrom(sourceType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
