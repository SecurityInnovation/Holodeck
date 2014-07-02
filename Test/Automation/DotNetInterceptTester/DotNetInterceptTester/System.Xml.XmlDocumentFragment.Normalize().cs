namespace DotNetInterceptTester.My_System.Xml.XmlDocumentFragment
{
public class Normalize_System_Xml_XmlDocumentFragment
{
public static bool _Normalize_System_Xml_XmlDocumentFragment( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocumentFragment.Normalize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocumentFragment.Normalize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
