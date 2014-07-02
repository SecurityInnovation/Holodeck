namespace DotNetInterceptTester.My_System.Xml.XmlNamedNodeMap
{
public class GetNamedItem_System_Xml_XmlNamedNodeMap_System_String
{
public static bool _GetNamedItem_System_Xml_XmlNamedNodeMap_System_String( )
{
   //Parameters
   System.String name = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNamedNodeMap.GetNamedItem(name);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNamedNodeMap.GetNamedItem(name);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
