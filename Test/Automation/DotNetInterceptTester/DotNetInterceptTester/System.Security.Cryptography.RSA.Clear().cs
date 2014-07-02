namespace DotNetInterceptTester.My_System.Security.Cryptography.RSA
{
public class Clear_System_Security_Cryptography_RSA
{
public static bool _Clear_System_Security_Cryptography_RSA( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSA.Clear();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSA.Clear();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
