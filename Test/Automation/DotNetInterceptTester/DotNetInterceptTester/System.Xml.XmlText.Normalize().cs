namespace DotNetInterceptTester.My_System.Xml.XmlText
{
public class Normalize_System_Xml_XmlText
{
public static bool _Normalize_System_Xml_XmlText( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlText.Normalize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlText.Normalize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
