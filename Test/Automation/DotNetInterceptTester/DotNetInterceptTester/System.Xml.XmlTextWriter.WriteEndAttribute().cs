namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteEndAttribute_System_Xml_XmlTextWriter
{
public static bool _WriteEndAttribute_System_Xml_XmlTextWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteEndAttribute();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteEndAttribute();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
