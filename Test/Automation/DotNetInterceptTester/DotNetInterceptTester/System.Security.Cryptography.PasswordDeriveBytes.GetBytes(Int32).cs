namespace DotNetInterceptTester.My_System.Security.Cryptography.PasswordDeriveBytes
{
public class GetBytes_System_Security_Cryptography_PasswordDeriveBytes_System_Int32
{
public static bool _GetBytes_System_Security_Cryptography_PasswordDeriveBytes_System_Int32( )
{
   //Parameters
   System.Int32 cb = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.PasswordDeriveBytes.GetBytes(cb);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.PasswordDeriveBytes.GetBytes(cb);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
