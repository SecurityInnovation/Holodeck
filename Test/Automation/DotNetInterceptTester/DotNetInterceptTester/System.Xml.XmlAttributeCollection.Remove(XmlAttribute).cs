namespace DotNetInterceptTester.My_System.Xml.XmlAttributeCollection
{
public class Remove_System_Xml_XmlAttributeCollection_System_Xml_XmlAttribute
{
public static bool _Remove_System_Xml_XmlAttributeCollection_System_Xml_XmlAttribute( )
{
   //Parameters
   System.Xml.XmlAttribute node = null;

   //ReturnType/Value
   System.Xml.XmlAttribute returnVal_Real = null;
   System.Xml.XmlAttribute returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlAttributeCollection.Remove(node);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlAttributeCollection.Remove(node);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
