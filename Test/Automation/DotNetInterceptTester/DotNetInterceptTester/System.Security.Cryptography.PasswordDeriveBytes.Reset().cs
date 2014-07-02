namespace DotNetInterceptTester.My_System.Security.Cryptography.PasswordDeriveBytes
{
public class Reset_System_Security_Cryptography_PasswordDeriveBytes
{
public static bool _Reset_System_Security_Cryptography_PasswordDeriveBytes( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.PasswordDeriveBytes.Reset();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.PasswordDeriveBytes.Reset();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
