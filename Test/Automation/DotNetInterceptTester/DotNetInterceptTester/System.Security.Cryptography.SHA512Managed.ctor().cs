namespace DotNetInterceptTester.My_System.Security.Cryptography.SHA512Managed
{
public class ctor_System_Security_Cryptography_SHA512Managed
{
public static bool _ctor_System_Security_Cryptography_SHA512Managed( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.SHA512Managed.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.SHA512Managed.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
