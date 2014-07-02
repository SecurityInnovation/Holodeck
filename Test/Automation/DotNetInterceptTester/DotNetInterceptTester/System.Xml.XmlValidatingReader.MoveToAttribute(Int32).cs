namespace DotNetInterceptTester.My_System.Xml.XmlValidatingReader
{
public class MoveToAttribute_System_Xml_XmlValidatingReader_System_Int32
{
public static bool _MoveToAttribute_System_Xml_XmlValidatingReader_System_Int32( )
{
   //Parameters
   System.Int32 i = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlValidatingReader.MoveToAttribute(i);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlValidatingReader.MoveToAttribute(i);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
