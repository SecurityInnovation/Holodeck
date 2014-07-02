namespace DotNetInterceptTester.My_System.Xml.XmlNodeReader
{
public class Skip_System_Xml_XmlNodeReader
{
public static bool _Skip_System_Xml_XmlNodeReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNodeReader.Skip();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNodeReader.Skip();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
