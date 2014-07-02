namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteStartDocument_System_Xml_XmlTextWriter_System_Boolean
{
public static bool _WriteStartDocument_System_Xml_XmlTextWriter_System_Boolean( )
{
   //Parameters
   System.Boolean standalone = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteStartDocument(standalone);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteStartDocument(standalone);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
