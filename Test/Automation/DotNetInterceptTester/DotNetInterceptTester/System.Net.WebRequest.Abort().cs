namespace DotNetInterceptTester.My_System.Net.WebRequest
{
public class Abort_System_Net_WebRequest
{
public static bool _Abort_System_Net_WebRequest( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebRequest.Abort();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebRequest.Abort();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
