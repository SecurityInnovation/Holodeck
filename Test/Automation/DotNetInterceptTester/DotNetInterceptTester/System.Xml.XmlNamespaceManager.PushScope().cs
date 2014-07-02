namespace DotNetInterceptTester.My_System.Xml.XmlNamespaceManager
{
public class PushScope_System_Xml_XmlNamespaceManager
{
public static bool _PushScope_System_Xml_XmlNamespaceManager( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNamespaceManager.PushScope();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNamespaceManager.PushScope();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
