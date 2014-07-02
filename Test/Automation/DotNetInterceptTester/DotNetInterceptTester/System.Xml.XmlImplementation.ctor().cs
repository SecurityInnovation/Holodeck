namespace DotNetInterceptTester.My_System.Xml.XmlImplementation
{
public class ctor_System_Xml_XmlImplementation
{
public static bool _ctor_System_Xml_XmlImplementation( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlImplementation.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlImplementation.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
