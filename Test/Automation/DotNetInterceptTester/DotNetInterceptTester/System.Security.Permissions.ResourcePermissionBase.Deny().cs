namespace DotNetInterceptTester.My_System.Security.Permissions.ResourcePermissionBase
{
public class Deny_System_Security_Permissions_ResourcePermissionBase
{
public static bool _Deny_System_Security_Permissions_ResourcePermissionBase( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Permissions.ResourcePermissionBase.Deny();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Permissions.ResourcePermissionBase.Deny();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
