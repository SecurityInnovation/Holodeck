namespace DotNetInterceptTester.My_System.Xml.XmlTextWriter
{
public class WriteSurrogateCharEntity_System_Xml_XmlTextWriter_System_Char_System_Char
{
public static bool _WriteSurrogateCharEntity_System_Xml_XmlTextWriter_System_Char_System_Char( )
{
   //Parameters
   System.Char lowChar = null;
   System.Char highChar = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlTextWriter.WriteSurrogateCharEntity(lowChar,highChar);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlTextWriter.WriteSurrogateCharEntity(lowChar,highChar);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
