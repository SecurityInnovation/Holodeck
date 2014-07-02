namespace DotNetInterceptTester.My_System.Net.WebClient
{
public class ctor_System_Net_WebClient
{
public static bool _ctor_System_Net_WebClient( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebClient.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebClient.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
