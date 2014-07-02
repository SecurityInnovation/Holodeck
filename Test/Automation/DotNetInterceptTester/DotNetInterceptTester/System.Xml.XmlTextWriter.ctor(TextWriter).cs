namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class ctor_System_Xml_XmlTextWriter_System_IO_TextWriter
{
public static bool _ctor_System_Xml_XmlTextWriter_System_IO_TextWriter( )
{
   //Parameters
   System.IO.TextWriter w = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.ctor(w);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.ctor(w);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
