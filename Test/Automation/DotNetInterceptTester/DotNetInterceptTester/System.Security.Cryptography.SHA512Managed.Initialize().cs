namespace DotNetInterceptTester.My_System.Security.Cryptography.SHA512Managed
{
public class Initialize_System_Security_Cryptography_SHA512Managed
{
public static bool _Initialize_System_Security_Cryptography_SHA512Managed( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.SHA512Managed.Initialize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.SHA512Managed.Initialize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
