namespace DotNetInterceptTester.My_System.Security.Cryptography.MACTripleDES
{
public class Initialize_System_Security_Cryptography_MACTripleDES
{
public static bool _Initialize_System_Security_Cryptography_MACTripleDES( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.MACTripleDES.Initialize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.MACTripleDES.Initialize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
