namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteWhitespace_System_Xml_XmlTextWriter_System_String
{
public static bool _WriteWhitespace_System_Xml_XmlTextWriter_System_String( )
{
   //Parameters
   System.String ws = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteWhitespace(ws);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteWhitespace(ws);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
