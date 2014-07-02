namespace DotNetInterceptTester.My_System.Xml.XmlNamedNodeMap
{
public class Item_System_Xml_XmlNamedNodeMap_System_Int32
{
public static bool _Item_System_Xml_XmlNamedNodeMap_System_Int32( )
{
   //Parameters
   System.Int32 index = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNamedNodeMap.Item(index);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNamedNodeMap.Item(index);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
