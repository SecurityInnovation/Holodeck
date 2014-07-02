namespace DotNetInterceptTester.My_System.Diagnostics.EventLogPermission
{
public class ctor_System_Diagnostics_EventLogPermission_System_Security_Permissions_PermissionState
{
public static bool _ctor_System_Diagnostics_EventLogPermission_System_Security_Permissions_PermissionState( )
{
   //Parameters
   System.Security.Permissions.PermissionState state = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogPermission.ctor(state);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogPermission.ctor(state);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
