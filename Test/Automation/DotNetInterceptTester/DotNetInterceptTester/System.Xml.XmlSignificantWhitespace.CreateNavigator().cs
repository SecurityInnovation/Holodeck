namespace DotNetInterceptTester.My_System.Xml.XmlSignificantWhitespace
{
public class CreateNavigator_System_Xml_XmlSignificantWhitespace
{
public static bool _CreateNavigator_System_Xml_XmlSignificantWhitespace( )
{
   //Parameters

   //ReturnType/Value
   System.Xml.XPath.XPathNavigator returnVal_Real = null;
   System.Xml.XPath.XPathNavigator returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlSignificantWhitespace.CreateNavigator();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlSignificantWhitespace.CreateNavigator();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
