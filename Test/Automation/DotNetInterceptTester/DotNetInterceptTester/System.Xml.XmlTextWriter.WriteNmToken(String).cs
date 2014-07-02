namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteNmToken_System_Xml_XmlTextWriter_System_String
{
public static bool _WriteNmToken_System_Xml_XmlTextWriter_System_String( )
{
   //Parameters
   System.String name = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteNmToken(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteNmToken(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
