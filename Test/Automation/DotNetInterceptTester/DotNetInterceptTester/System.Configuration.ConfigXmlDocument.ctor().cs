namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class ctor_System_Configuration_ConfigXmlDocument
{
public static bool _ctor_System_Configuration_ConfigXmlDocument( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
