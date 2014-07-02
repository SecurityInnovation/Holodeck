namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteCharEntity_System_Xml_XmlTextWriter_System_Char
{
public static bool _WriteCharEntity_System_Xml_XmlTextWriter_System_Char( )
{
   //Parameters
   System.Char ch = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteCharEntity(ch);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteCharEntity(ch);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
