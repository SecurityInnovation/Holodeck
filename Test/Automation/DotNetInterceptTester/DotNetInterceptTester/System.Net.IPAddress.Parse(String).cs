namespace DotNetInterceptTester.My_System.Net.IPAddress
{
public class Parse_System_String
{
public static bool _Parse_System_String( )
{
   //Parameters
   System.String ipString = null;

   //ReturnType/Value
   System.Net.IPAddress returnVal_Real = null;
   System.Net.IPAddress returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.IPAddress.Parse(ipString);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.IPAddress.Parse(ipString);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
