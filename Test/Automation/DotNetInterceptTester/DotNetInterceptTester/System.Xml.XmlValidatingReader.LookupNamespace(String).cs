namespace DotNetInterceptTester.My_System.Xml.XmlValidatingReader
{
public class LookupNamespace_System_Xml_XmlValidatingReader_System_String
{
public static bool _LookupNamespace_System_Xml_XmlValidatingReader_System_String( )
{
   //Parameters
   System.String prefix = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlValidatingReader.LookupNamespace(prefix);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlValidatingReader.LookupNamespace(prefix);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
