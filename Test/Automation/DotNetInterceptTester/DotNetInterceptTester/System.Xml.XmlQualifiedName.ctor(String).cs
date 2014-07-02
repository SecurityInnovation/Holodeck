namespace DotNetInterceptTester.My_System.Xml.XmlQualifiedName
{
public class ctor_System_Xml_XmlQualifiedName_System_String
{
public static bool _ctor_System_Xml_XmlQualifiedName_System_String( )
{
   //Parameters
   System.String name = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlQualifiedName.ctor(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlQualifiedName.ctor(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
