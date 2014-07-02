namespace DotNetInterceptTester.My_System.Net.HttpWebRequest
{
public class BeginGetResponse_System_Net_HttpWebRequest_System_AsyncCallback_System_Object
{
public static bool _BeginGetResponse_System_Net_HttpWebRequest_System_AsyncCallback_System_Object( )
{
   //Parameters
   System.AsyncCallback callback = null;
   System.Object state = null;

   //ReturnType/Value
   System.IAsyncResult returnVal_Real = null;
   System.IAsyncResult returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.HttpWebRequest.BeginGetResponse(callback,state);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.HttpWebRequest.BeginGetResponse(callback,state);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
