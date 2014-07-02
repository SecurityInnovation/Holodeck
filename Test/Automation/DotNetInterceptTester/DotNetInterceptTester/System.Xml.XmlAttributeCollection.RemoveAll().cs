namespace DotNetInterceptTester.My_System.Xml.XmlAttributeCollection
{
public class RemoveAll_System_Xml_XmlAttributeCollection
{
public static bool _RemoveAll_System_Xml_XmlAttributeCollection( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlAttributeCollection.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlAttributeCollection.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
