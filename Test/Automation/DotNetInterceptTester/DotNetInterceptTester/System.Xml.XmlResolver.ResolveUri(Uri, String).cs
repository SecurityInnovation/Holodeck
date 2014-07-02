namespace DotNetInterceptTester.My_System.Xml.XmlResolver
{
public class ResolveUri_System_Xml_XmlResolver_System_Uri_System_String
{
public static bool _ResolveUri_System_Xml_XmlResolver_System_Uri_System_String( )
{
   //Parameters
   System.Uri baseUri = null;
   System.String relativeUri = null;

   //ReturnType/Value
   System.Uri returnVal_Real = null;
   System.Uri returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlResolver.ResolveUri(baseUri,relativeUri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlResolver.ResolveUri(baseUri,relativeUri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
