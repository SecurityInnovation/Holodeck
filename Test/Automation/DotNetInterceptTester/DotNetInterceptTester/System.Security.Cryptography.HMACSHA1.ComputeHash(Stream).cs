namespace DotNetInterceptTester.My_System.Security.Cryptography.HMACSHA1
{
public class ComputeHash_System_Security_Cryptography_HMACSHA1_System_IO_Stream
{
public static bool _ComputeHash_System_Security_Cryptography_HMACSHA1_System_IO_Stream( )
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
      returnValue_Real = System.Security.Cryptography.HMACSHA1.ComputeHash(inputStream);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.HMACSHA1.ComputeHash(inputStream);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
