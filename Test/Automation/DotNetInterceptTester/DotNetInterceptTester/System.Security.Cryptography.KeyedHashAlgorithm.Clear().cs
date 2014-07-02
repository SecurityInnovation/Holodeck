namespace DotNetInterceptTester.My_System.Security.Cryptography.KeyedHashAlgorithm
{
public class Clear_System_Security_Cryptography_KeyedHashAlgorithm
{
public static bool _Clear_System_Security_Cryptography_KeyedHashAlgorithm( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.KeyedHashAlgorithm.Clear();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.KeyedHashAlgorithm.Clear();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
