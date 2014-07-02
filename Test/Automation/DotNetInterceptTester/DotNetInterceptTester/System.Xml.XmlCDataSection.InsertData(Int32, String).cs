namespace DotNetInterceptTester.My_System.Xml.XmlCDataSection
{
public class InsertData_System_Xml_XmlCDataSection_System_Int32_System_String
{
public static bool _InsertData_System_Xml_XmlCDataSection_System_Int32_System_String( )
{
   //Parameters
   System.Int32 offset = null;
   System.String strData = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlCDataSection.InsertData(offset,strData);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlCDataSection.InsertData(offset,strData);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
