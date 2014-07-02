namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoStream
{
public class EndWrite_System_Security_Cryptography_CryptoStream_System_IAsyncResult
{
public static bool _EndWrite_System_Security_Cryptography_CryptoStream_System_IAsyncResult( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoStream.EndWrite(asyncResult);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoStream.EndWrite(asyncResult);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
