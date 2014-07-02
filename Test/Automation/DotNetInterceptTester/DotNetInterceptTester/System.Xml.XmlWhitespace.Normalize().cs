namespace DotNetInterceptTester.My_System.Xml.XmlWhitespace
{
public class Normalize_System_Xml_XmlWhitespace
{
public static bool _Normalize_System_Xml_XmlWhitespace( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlWhitespace.Normalize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlWhitespace.Normalize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
