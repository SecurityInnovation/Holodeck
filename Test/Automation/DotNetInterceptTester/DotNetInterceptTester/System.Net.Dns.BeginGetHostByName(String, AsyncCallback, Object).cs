namespace DotNetInterceptTester.My_System.Net.Dns
{
public class BeginGetHostByName_System_String_System_AsyncCallback_System_Object
{
public static bool _BeginGetHostByName_System_String_System_AsyncCallback_System_Object( )
{
   //Parameters
   System.String hostName = null;
   System.AsyncCallback requestCallback = null;
   System.Object stateObject = null;

   //ReturnType/Value
   System.IAsyncResult returnVal_Real = null;
   System.IAsyncResult returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Dns.BeginGetHostByName(hostName,requestCallback,stateObject);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Dns.BeginGetHostByName(hostName,requestCallback,stateObject);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
