namespace DotNetInterceptTester.My_System.Xml.XmlTextReader
{
public class MoveToContent_System_Xml_XmlTextReader
{
public static bool _MoveToContent_System_Xml_XmlTextReader( )
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
      returnValue_Real = System.Xml.XmlTextReader.MoveToContent();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextReader.MoveToContent();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
