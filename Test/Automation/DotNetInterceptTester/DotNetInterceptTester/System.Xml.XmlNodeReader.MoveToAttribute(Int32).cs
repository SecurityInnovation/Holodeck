namespace DotNetInterceptTester.My_System.Xml.XmlNodeReader
{
public class MoveToAttribute_System_Xml_XmlNodeReader_System_Int32
{
public static bool _MoveToAttribute_System_Xml_XmlNodeReader_System_Int32( )
{
   //Parameters
   System.Int32 attributeIndex = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNodeReader.MoveToAttribute(attributeIndex);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNodeReader.MoveToAttribute(attributeIndex);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
