namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class Save_System_Configuration_ConfigXmlDocument_System_IO_Stream
{
public static bool _Save_System_Configuration_ConfigXmlDocument_System_IO_Stream( )
{
   //Parameters
   System.IO.Stream outStream = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.Save(outStream);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.Save(outStream);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
