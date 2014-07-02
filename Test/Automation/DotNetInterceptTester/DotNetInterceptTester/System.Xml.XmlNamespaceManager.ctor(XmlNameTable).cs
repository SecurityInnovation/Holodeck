namespace DotNetInterceptTester.My_System.Xml.XmlNamespaceManager
{
public class ctor_System_Xml_XmlNamespaceManager_System_Xml_XmlNameTable
{
public static bool _ctor_System_Xml_XmlNamespaceManager_System_Xml_XmlNameTable( )
{
   //Parameters
   System.Xml.XmlNameTable nameTable = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNamespaceManager.ctor(nameTable);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNamespaceManager.ctor(nameTable);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
