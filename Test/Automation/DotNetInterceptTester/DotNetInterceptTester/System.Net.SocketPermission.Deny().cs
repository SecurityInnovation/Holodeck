namespace DotNetInterceptTester.My_System.Net.SocketPermission
{
public class Deny_System_Net_SocketPermission
{
public static bool _Deny_System_Net_SocketPermission( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.SocketPermission.Deny();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.SocketPermission.Deny();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
