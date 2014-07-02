namespace DotNetInterceptTester.My_System.Xml.XmlEntity
{
public class WriteTo_System_Xml_XmlEntity_System_Xml_XmlWriter
{
public static bool _WriteTo_System_Xml_XmlEntity_System_Xml_XmlWriter( )
{
   //Parameters
   System.Xml.XmlWriter w = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlEntity.WriteTo(w);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlEntity.WriteTo(w);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
