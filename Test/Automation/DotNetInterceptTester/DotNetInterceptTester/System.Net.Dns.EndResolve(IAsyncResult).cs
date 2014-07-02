namespace DotNetInterceptTester.My_System.Net.Dns
{
public class EndResolve_System_IAsyncResult
{
public static bool _EndResolve_System_IAsyncResult( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;

   //ReturnType/Value
   System.Net.IPHostEntry returnVal_Real = null;
   System.Net.IPHostEntry returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Dns.EndResolve(asyncResult);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Dns.EndResolve(asyncResult);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
