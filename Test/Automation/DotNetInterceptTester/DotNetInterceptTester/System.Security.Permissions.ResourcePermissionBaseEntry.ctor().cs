namespace DotNetInterceptTester.My_System.Security.Permissions.ResourcePermissionBaseEntry
{
public class ctor_System_Security_Permissions_ResourcePermissionBaseEntry
{
public static bool _ctor_System_Security_Permissions_ResourcePermissionBaseEntry( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Permissions.ResourcePermissionBaseEntry.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Permissions.ResourcePermissionBaseEntry.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
