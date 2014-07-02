namespace DotNetInterceptTester.My_System.Xml.XmlValidatingReader
{
public class ResolveEntity_System_Xml_XmlValidatingReader
{
public static bool _ResolveEntity_System_Xml_XmlValidatingReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlValidatingReader.ResolveEntity();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlValidatingReader.ResolveEntity();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
