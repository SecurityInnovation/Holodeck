namespace DotNetInterceptTester.My_System.Xml.XmlUrlResolver
{
public class GetEntity_System_Xml_XmlUrlResolver_System_Uri_System_String_System_Type
{
public static bool _GetEntity_System_Xml_XmlUrlResolver_System_Uri_System_String_System_Type( )
{
   //Parameters
   System.Uri absoluteUri = null;
   System.String role = null;
   System.Type ofObjectToReturn = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlUrlResolver.GetEntity(absoluteUri,role,ofObjectToReturn);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlUrlResolver.GetEntity(absoluteUri,role,ofObjectToReturn);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
