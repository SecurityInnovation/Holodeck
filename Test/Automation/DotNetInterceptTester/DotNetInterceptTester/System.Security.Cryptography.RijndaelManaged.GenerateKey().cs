namespace DotNetInterceptTester.My_System.Security.Cryptography.RijndaelManaged
{
public class GenerateKey_System_Security_Cryptography_RijndaelManaged
{
public static bool _GenerateKey_System_Security_Cryptography_RijndaelManaged( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RijndaelManaged.GenerateKey();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RijndaelManaged.GenerateKey();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
