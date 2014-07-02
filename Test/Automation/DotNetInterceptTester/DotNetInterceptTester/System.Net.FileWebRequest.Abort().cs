namespace DotNetInterceptTester.My_System.Net.FileWebRequest
{
public class Abort_System_Net_FileWebRequest
{
public static bool _Abort_System_Net_FileWebRequest( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.FileWebRequest.Abort();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.FileWebRequest.Abort();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
