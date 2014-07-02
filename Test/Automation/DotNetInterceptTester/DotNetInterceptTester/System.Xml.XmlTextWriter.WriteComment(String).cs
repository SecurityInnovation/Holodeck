namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteComment_System_Xml_XmlTextWriter_System_String
{
public static bool _WriteComment_System_Xml_XmlTextWriter_System_String( )
{
   //Parameters
   System.String text = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteComment(text);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteComment(text);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
