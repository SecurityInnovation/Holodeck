namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoStream
{
public class EndRead_System_Security_Cryptography_CryptoStream_System_IAsyncResult
{
public static bool _EndRead_System_Security_Cryptography_CryptoStream_System_IAsyncResult( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;

   //ReturnType/Value
   System.Int32 returnVal_Real = null;
   System.Int32 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoStream.EndRead(asyncResult);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoStream.EndRead(asyncResult);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
