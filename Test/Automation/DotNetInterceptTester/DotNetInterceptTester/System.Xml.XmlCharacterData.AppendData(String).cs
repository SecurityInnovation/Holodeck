namespace DotNetInterceptTester.My_System.Xml.XmlCharacterData
{
public class AppendData_System_Xml_XmlCharacterData_System_String
{
public static bool _AppendData_System_Xml_XmlCharacterData_System_String( )
{
   //Parameters
   System.String strData = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlCharacterData.AppendData(strData);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlCharacterData.AppendData(strData);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
