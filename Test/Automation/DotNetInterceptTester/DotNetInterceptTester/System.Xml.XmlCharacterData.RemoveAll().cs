namespace DotNetInterceptTester.My_System.Xml.XmlCharacterData
{
public class RemoveAll_System_Xml_XmlCharacterData
{
public static bool _RemoveAll_System_Xml_XmlCharacterData( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlCharacterData.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlCharacterData.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
