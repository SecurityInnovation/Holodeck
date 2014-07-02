namespace DotNetInterceptTester.My_System.Xml.XmlValidatingReader
{
public class ctor_System_Xml_XmlValidatingReader_System_Xml_XmlReader
{
public static bool _ctor_System_Xml_XmlValidatingReader_System_Xml_XmlReader( )
{
   //Parameters
   System.Xml.XmlReader reader = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlValidatingReader.ctor(reader);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlValidatingReader.ctor(reader);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
