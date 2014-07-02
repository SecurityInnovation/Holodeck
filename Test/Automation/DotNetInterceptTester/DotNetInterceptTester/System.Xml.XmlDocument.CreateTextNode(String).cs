namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class CreateTextNode_System_Xml_XmlDocument_System_String
{
public static bool _CreateTextNode_System_Xml_XmlDocument_System_String( )
{
   //Parameters
   System.String text = null;

   //ReturnType/Value
   System.Xml.XmlText returnVal_Real = null;
   System.Xml.XmlText returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.CreateTextNode(text);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.CreateTextNode(text);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
