namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class Load_System_Configuration_ConfigXmlDocument_System_String
{
public static bool _Load_System_Configuration_ConfigXmlDocument_System_String( )
{
   //Parameters
   System.String filename = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.Load(filename);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.Load(filename);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
