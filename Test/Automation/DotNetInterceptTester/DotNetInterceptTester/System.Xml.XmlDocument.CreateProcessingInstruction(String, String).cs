namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class CreateProcessingInstruction_System_Xml_XmlDocument_System_String_System_String
{
public static bool _CreateProcessingInstruction_System_Xml_XmlDocument_System_String_System_String( )
{
   //Parameters
   System.String target = null;
   System.String data = null;

   //ReturnType/Value
   System.Xml.XmlProcessingInstruction returnVal_Real = null;
   System.Xml.XmlProcessingInstruction returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.CreateProcessingInstruction(target,data);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.CreateProcessingInstruction(target,data);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
