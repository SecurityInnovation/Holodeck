namespace DotNetInterceptTester.My_System.Net.HttpWebRequest
{
public class Abort_System_Net_HttpWebRequest
{
public static bool _Abort_System_Net_HttpWebRequest( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.HttpWebRequest.Abort();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.HttpWebRequest.Abort();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
