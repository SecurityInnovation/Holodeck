namespace DotNetInterceptTester.My_System.Xml.XmlWhitespace
{
public class WriteContentTo_System_Xml_XmlWhitespace_System_Xml_XmlWriter
{
public static bool _WriteContentTo_System_Xml_XmlWhitespace_System_Xml_XmlWriter( )
{
   //Parameters
   System.Xml.XmlWriter w = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlWhitespace.WriteContentTo(w);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlWhitespace.WriteContentTo(w);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
