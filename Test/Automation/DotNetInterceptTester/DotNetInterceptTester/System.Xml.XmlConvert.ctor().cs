namespace DotNetInterceptTester.My_System.Xml.XmlConvert
{
public class ctor_System_Xml_XmlConvert
{
public static bool _ctor_System_Xml_XmlConvert( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlConvert.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlConvert.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
