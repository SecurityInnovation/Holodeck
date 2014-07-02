namespace DotNetInterceptTester.My_System.Net.AuthenticationManager
{
public class PreAuthenticate_System_Net_WebRequest_System_Net_ICredentials
{
public static bool _PreAuthenticate_System_Net_WebRequest_System_Net_ICredentials( )
{
   //Parameters
   System.Net.WebRequest request = null;
   System.Net.ICredentials credentials = null;

   //ReturnType/Value
   System.Net.Authorization returnVal_Real = null;
   System.Net.Authorization returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.AuthenticationManager.PreAuthenticate(request,credentials);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.AuthenticationManager.PreAuthenticate(request,credentials);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
