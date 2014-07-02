namespace DotNetInterceptTester.My_System.Xml.XmlLinkedNode
{
public class RemoveAll_System_Xml_XmlLinkedNode
{
public static bool _RemoveAll_System_Xml_XmlLinkedNode( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlLinkedNode.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlLinkedNode.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
