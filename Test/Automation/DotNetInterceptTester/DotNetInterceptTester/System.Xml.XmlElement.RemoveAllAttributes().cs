namespace DotNetInterceptTester.My_System.Xml.XmlElement
{
public class RemoveAllAttributes_System_Xml_XmlElement
{
public static bool _RemoveAllAttributes_System_Xml_XmlElement( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlElement.RemoveAllAttributes();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlElement.RemoveAllAttributes();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
