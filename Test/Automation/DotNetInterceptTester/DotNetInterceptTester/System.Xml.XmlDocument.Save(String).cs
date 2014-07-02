namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class Save_System_Xml_XmlDocument_System_String
{
public static bool _Save_System_Xml_XmlDocument_System_String( )
{
   //Parameters
   System.String filename = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.Save(filename);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.Save(filename);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
