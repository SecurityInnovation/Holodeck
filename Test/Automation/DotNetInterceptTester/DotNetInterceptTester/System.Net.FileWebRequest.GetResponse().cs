namespace DotNetInterceptTester.My_System.Net.FileWebRequest
{
public class GetResponse_System_Net_FileWebRequest
{
public static bool _GetResponse_System_Net_FileWebRequest( )
{
   //Parameters

   //ReturnType/Value
   System.Net.WebResponse returnVal_Real = null;
   System.Net.WebResponse returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.FileWebRequest.GetResponse();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.FileWebRequest.GetResponse();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
