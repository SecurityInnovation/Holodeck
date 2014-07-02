namespace DotNetInterceptTester.My_System.Xml.XmlSignificantWhitespace
{
public class RemoveAll_System_Xml_XmlSignificantWhitespace
{
public static bool _RemoveAll_System_Xml_XmlSignificantWhitespace( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlSignificantWhitespace.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlSignificantWhitespace.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
