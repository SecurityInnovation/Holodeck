namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class GetElementsByTagName_System_Configuration_ConfigXmlDocument_System_String
{
public static bool _GetElementsByTagName_System_Configuration_ConfigXmlDocument_System_String( )
{
   //Parameters
   System.String name = null;

   //ReturnType/Value
   System.Xml.XmlNodeList returnVal_Real = null;
   System.Xml.XmlNodeList returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.GetElementsByTagName(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.GetElementsByTagName(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
