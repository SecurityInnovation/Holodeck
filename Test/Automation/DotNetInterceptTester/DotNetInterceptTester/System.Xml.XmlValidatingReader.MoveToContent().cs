namespace DotNetInterceptTester.My_System.Xml.XmlValidatingReader
{
public class MoveToContent_System_Xml_XmlValidatingReader
{
public static bool _MoveToContent_System_Xml_XmlValidatingReader( )
{
   //Parameters

   //ReturnType/Value
   System.Xml.XmlNodeType returnVal_Real = null;
   System.Xml.XmlNodeType returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlValidatingReader.MoveToContent();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlValidatingReader.MoveToContent();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
