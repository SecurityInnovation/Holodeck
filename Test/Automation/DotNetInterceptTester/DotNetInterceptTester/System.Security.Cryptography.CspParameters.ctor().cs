namespace DotNetInterceptTester.My_System.Security.Cryptography.CspParameters
{
public class ctor_System_Security_Cryptography_CspParameters
{
public static bool _ctor_System_Security_Cryptography_CspParameters( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CspParameters.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CspParameters.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
