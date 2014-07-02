namespace DotNetInterceptTester.My_System.Xml.XmlAttributeCollection
{
public class RemoveAt_System_Xml_XmlAttributeCollection_System_Int32
{
public static bool _RemoveAt_System_Xml_XmlAttributeCollection_System_Int32( )
{
   //Parameters
   System.Int32 i = null;

   //ReturnType/Value
   System.Xml.XmlAttribute returnVal_Real = null;
   System.Xml.XmlAttribute returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlAttributeCollection.RemoveAt(i);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlAttributeCollection.RemoveAt(i);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
