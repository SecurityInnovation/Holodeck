namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteDocType_System_Xml_XmlTextWriter_System_String_System_String_System_String_System_String
{
public static bool _WriteDocType_System_Xml_XmlTextWriter_System_String_System_String_System_String_System_String( )
{
   //Parameters
   System.String name = null;
   System.String pubid = null;
   System.String sysid = null;
   System.String subset = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteDocType(name,pubid,sysid,subset);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteDocType(name,pubid,sysid,subset);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
