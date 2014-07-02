namespace DotNetInterceptTester.My_System.Xml.XmlTextReader
{
public class ctor_System_Xml_XmlTextReader_System_IO_Stream_System_Xml_XmlNameTable
{
public static bool _ctor_System_Xml_XmlTextReader_System_IO_Stream_System_Xml_XmlNameTable( )
{
   //Parameters
   System.IO.Stream input = null;
   System.Xml.XmlNameTable nt = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextReader.ctor(input,nt);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextReader.ctor(input,nt);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
