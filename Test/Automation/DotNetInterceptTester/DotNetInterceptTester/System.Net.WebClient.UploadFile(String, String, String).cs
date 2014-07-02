namespace DotNetInterceptTester.My_System.Net.WebClient
{
public class UploadFile_System_Net_WebClient_System_String_System_String_System_String
{
public static bool _UploadFile_System_Net_WebClient_System_String_System_String_System_String( )
{
   //Parameters
   System.String address = null;
   System.String method = null;
   System.String fileName = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebClient.UploadFile(address,method,fileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebClient.UploadFile(address,method,fileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
