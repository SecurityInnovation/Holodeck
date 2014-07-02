namespace DotNetInterceptTester.My_System.Net.WebProxy
{
public class GetProxy_System_Net_WebProxy_System_Uri
{
public static bool _GetProxy_System_Net_WebProxy_System_Uri( )
{
   //Parameters
   System.Uri destination = null;

   //ReturnType/Value
   System.Uri returnVal_Real = null;
   System.Uri returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebProxy.GetProxy(destination);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebProxy.GetProxy(destination);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
