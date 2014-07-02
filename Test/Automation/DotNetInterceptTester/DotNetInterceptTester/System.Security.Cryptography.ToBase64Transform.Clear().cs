namespace DotNetInterceptTester.My_System.Security.Cryptography.ToBase64Transform
{
public class Clear_System_Security_Cryptography_ToBase64Transform
{
public static bool _Clear_System_Security_Cryptography_ToBase64Transform( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.ToBase64Transform.Clear();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.ToBase64Transform.Clear();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
