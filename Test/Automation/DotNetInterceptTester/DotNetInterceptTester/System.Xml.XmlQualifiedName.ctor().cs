namespace DotNetInterceptTester.My_System.Xml.XmlQualifiedName
{
public class ctor_System_Xml_XmlQualifiedName
{
public static bool _ctor_System_Xml_XmlQualifiedName( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlQualifiedName.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlQualifiedName.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
