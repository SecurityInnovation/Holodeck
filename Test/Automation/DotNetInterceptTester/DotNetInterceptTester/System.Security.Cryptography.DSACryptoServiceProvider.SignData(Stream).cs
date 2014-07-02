namespace DotNetInterceptTester.My_System.Security.Cryptography.DSACryptoServiceProvider
{
public class SignData_System_Security_Cryptography_DSACryptoServiceProvider_System_IO_Stream
{
public static bool _SignData_System_Security_Cryptography_DSACryptoServiceProvider_System_IO_Stream( )
{
   //Parameters
   System.IO.Stream inputStream = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DSACryptoServiceProvider.SignData(inputStream);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DSACryptoServiceProvider.SignData(inputStream);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
