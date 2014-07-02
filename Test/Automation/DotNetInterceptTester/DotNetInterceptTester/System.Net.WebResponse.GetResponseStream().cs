namespace DotNetInterceptTester.My_System.Net.WebResponse
{
public class GetResponseStream_System_Net_WebResponse
{
public static bool _GetResponseStream_System_Net_WebResponse( )
{
   //Parameters

   //ReturnType/Value
   System.IO.Stream returnVal_Real = null;
   System.IO.Stream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebResponse.GetResponseStream();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebResponse.GetResponseStream();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
