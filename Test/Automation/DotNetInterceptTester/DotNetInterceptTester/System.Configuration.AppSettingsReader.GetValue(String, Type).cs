namespace DotNetInterceptTester.My_System.Configuration.AppSettingsReader
{
public class GetValue_System_Configuration_AppSettingsReader_System_String_System_Type
{
public static bool _GetValue_System_Configuration_AppSettingsReader_System_String_System_Type( )
{
   //Parameters
   System.String key = null;
   System.Type type = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.AppSettingsReader.GetValue(key,type);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.AppSettingsReader.GetValue(key,type);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
