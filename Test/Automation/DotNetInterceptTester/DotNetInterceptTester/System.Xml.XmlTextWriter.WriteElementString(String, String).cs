namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteElementString_System_Xml_XmlTextWriter_System_String_System_String
{
public static bool _WriteElementString_System_Xml_XmlTextWriter_System_String_System_String( )
{
   //Parameters
   System.String localName = null;
   System.String _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteElementString(localName,_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteElementString(localName,_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
