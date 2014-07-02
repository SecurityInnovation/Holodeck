namespace DotNetInterceptTester.My_System.Xml.XmlText
{
public class SplitText_System_Xml_XmlText_System_Int32
{
public static bool _SplitText_System_Xml_XmlText_System_Int32( )
{
   //Parameters
   System.Int32 offset = null;

   //ReturnType/Value
   System.Xml.XmlText returnVal_Real = null;
   System.Xml.XmlText returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlText.SplitText(offset);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlText.SplitText(offset);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
