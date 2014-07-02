namespace DotNetInterceptTester.My_System.Xml.XmlConvert
{
public class ToByte_System_String
{
public static bool _ToByte_System_String( )
{
   //Parameters
   System.String s = null;

   //ReturnType/Value
   System.Byte returnVal_Real = null;
   System.Byte returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlConvert.ToByte(s);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlConvert.ToByte(s);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
