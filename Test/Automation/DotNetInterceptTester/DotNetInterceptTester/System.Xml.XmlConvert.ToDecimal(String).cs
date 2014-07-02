namespace DotNetInterceptTester.My_System.Xml.XmlConvert
{
public class ToDecimal_System_String
{
public static bool _ToDecimal_System_String( )
{
   //Parameters
   System.String s = null;

   //ReturnType/Value
   System.Decimal returnVal_Real = null;
   System.Decimal returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlConvert.ToDecimal(s);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlConvert.ToDecimal(s);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
