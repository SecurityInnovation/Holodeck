namespace DotNetInterceptTester.My_System.Xml.XmlEntityReference
{
public class Normalize_System_Xml_XmlEntityReference
{
public static bool _Normalize_System_Xml_XmlEntityReference( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlEntityReference.Normalize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlEntityReference.Normalize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
