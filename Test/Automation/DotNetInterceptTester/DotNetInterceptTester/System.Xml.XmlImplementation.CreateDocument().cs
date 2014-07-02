namespace DotNetInterceptTester.My_System.Xml.XmlImplementation
{
public class CreateDocument_System_Xml_XmlImplementation
{
public static bool _CreateDocument_System_Xml_XmlImplementation( )
{
   //Parameters

   //ReturnType/Value
   System.Xml.XmlDocument returnVal_Real = null;
   System.Xml.XmlDocument returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlImplementation.CreateDocument();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlImplementation.CreateDocument();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
