namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class CreateWhitespace_System_Configuration_ConfigXmlDocument_System_String
{
public static bool _CreateWhitespace_System_Configuration_ConfigXmlDocument_System_String( )
{
   //Parameters
   System.String data = null;

   //ReturnType/Value
   System.Xml.XmlWhitespace returnVal_Real = null;
   System.Xml.XmlWhitespace returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.CreateWhitespace(data);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.CreateWhitespace(data);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
