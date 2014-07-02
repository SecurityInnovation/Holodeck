namespace DotNetInterceptTester.My_System.Xml.XmlNodeReader
{
public class Close_System_Xml_XmlNodeReader
{
public static bool _Close_System_Xml_XmlNodeReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNodeReader.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNodeReader.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
