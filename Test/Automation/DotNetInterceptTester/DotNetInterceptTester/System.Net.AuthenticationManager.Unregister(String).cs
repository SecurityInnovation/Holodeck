namespace DotNetInterceptTester.My_System.Net.AuthenticationManager
{
public class Unregister_System_String
{
public static bool _Unregister_System_String( )
{
   //Parameters
   System.String authenticationScheme = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.AuthenticationManager.Unregister(authenticationScheme);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.AuthenticationManager.Unregister(authenticationScheme);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
