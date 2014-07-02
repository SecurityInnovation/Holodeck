namespace DotNetInterceptTester.My_System.Xml.XmlTextReader
{
public class GetRemainder_System_Xml_XmlTextReader
{
public static bool _GetRemainder_System_Xml_XmlTextReader( )
{
   //Parameters

   //ReturnType/Value
   System.IO.TextReader returnVal_Real = null;
   System.IO.TextReader returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextReader.GetRemainder();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextReader.GetRemainder();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
