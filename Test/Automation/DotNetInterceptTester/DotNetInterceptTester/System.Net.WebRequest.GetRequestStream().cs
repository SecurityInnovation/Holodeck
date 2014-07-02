namespace DotNetInterceptTester.My_System.Net.WebRequest
{
public class GetRequestStream_System_Net_WebRequest
{
public static bool _GetRequestStream_System_Net_WebRequest( )
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
      returnValue_Real = System.Net.WebRequest.GetRequestStream();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebRequest.GetRequestStream();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
