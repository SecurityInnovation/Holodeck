namespace DotNetInterceptTester.My_System.Xml.XmlAttribute
{
public class Normalize_System_Xml_XmlAttribute
{
public static bool _Normalize_System_Xml_XmlAttribute( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlAttribute.Normalize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlAttribute.Normalize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
