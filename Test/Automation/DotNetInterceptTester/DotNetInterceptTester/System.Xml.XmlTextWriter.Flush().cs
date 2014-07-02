namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class Flush_System_Xml_XmlTextWriter
{
public static bool _Flush_System_Xml_XmlTextWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
