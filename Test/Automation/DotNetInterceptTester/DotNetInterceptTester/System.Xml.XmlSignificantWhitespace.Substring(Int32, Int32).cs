namespace DotNetInterceptTester.My_System.Xml.XmlSignificantWhitespace
{
public class Substring_System_Xml_XmlSignificantWhitespace_System_Int32_System_Int32
{
public static bool _Substring_System_Xml_XmlSignificantWhitespace_System_Int32_System_Int32( )
{
   //Parameters
   System.Int32 offset = null;
   System.Int32 count = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlSignificantWhitespace.Substring(offset,count);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlSignificantWhitespace.Substring(offset,count);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
