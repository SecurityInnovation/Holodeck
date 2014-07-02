namespace DotNetInterceptTester.My_System.Xml.XmlElement
{
public class RemoveAttribute_System_Xml_XmlElement_System_String
{
public static bool _RemoveAttribute_System_Xml_XmlElement_System_String( )
{
   //Parameters
   System.String name = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlElement.RemoveAttribute(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlElement.RemoveAttribute(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
