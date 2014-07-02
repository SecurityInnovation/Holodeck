namespace DotNetInterceptTester.My_System.Xml.XmlTextReader
{
public class ctor_System_Xml_XmlTextReader_System_String
{
public static bool _ctor_System_Xml_XmlTextReader_System_String( )
{
   //Parameters
   System.String url = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextReader.ctor(url);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextReader.ctor(url);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
