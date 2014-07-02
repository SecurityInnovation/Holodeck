namespace DotNetInterceptTester.My_System.Net.WebClient
{
public class OpenWrite_System_Net_WebClient_System_String_System_String
{
public static bool _OpenWrite_System_Net_WebClient_System_String_System_String( )
{
   //Parameters
   System.String address = null;
   System.String method = null;

   //ReturnType/Value
   System.IO.Stream returnVal_Real = null;
   System.IO.Stream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebClient.OpenWrite(address,method);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebClient.OpenWrite(address,method);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
