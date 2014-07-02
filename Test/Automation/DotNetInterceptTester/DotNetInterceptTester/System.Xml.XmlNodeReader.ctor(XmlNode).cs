namespace DotNetInterceptTester.My_System.Xml.XmlNodeReader
{
public class ctor_System_Xml_XmlNodeReader_System_Xml_XmlNode
{
public static bool _ctor_System_Xml_XmlNodeReader_System_Xml_XmlNode( )
{
   //Parameters
   System.Xml.XmlNode node = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNodeReader.ctor(node);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNodeReader.ctor(node);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
