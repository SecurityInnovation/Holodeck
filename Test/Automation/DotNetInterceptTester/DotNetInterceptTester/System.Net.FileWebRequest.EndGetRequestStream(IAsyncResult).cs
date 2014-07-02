namespace DotNetInterceptTester.My_System.Net.FileWebRequest
{
public class EndGetRequestStream_System_Net_FileWebRequest_System_IAsyncResult
{
public static bool _EndGetRequestStream_System_Net_FileWebRequest_System_IAsyncResult( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;

   //ReturnType/Value
   System.IO.Stream returnVal_Real = null;
   System.IO.Stream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.FileWebRequest.EndGetRequestStream(asyncResult);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.FileWebRequest.EndGetRequestStream(asyncResult);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
