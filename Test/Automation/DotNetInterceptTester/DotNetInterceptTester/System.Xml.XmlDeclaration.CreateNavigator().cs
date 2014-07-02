namespace DotNetInterceptTester.My_System.Xml.XmlDeclaration
{
public class CreateNavigator_System_Xml_XmlDeclaration
{
public static bool _CreateNavigator_System_Xml_XmlDeclaration( )
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
      returnValue_Real = System.Xml.XmlDeclaration.CreateNavigator();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDeclaration.CreateNavigator();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
