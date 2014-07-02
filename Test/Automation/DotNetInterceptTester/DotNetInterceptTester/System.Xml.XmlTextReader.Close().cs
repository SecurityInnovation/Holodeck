namespace DotNetInterceptTester.My_System.Xml.XmlTextReader
{
public class Close_System_Xml_XmlTextReader
{
public static bool _Close_System_Xml_XmlTextReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextReader.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextReader.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
