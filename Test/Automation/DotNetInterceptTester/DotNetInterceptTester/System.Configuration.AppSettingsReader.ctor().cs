namespace DotNetInterceptTester.My_System.Configuration.AppSettingsReader
{
public class ctor_System_Configuration_AppSettingsReader
{
public static bool _ctor_System_Configuration_AppSettingsReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.AppSettingsReader.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.AppSettingsReader.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
