namespace DotNetInterceptTester.My_System.Xml.XmlEntityReference
{
public class RemoveAll_System_Xml_XmlEntityReference
{
public static bool _RemoveAll_System_Xml_XmlEntityReference( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlEntityReference.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlEntityReference.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
