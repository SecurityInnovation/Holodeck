namespace DotNetInterceptTester.My_System.Diagnostics.EventLogPermission
{
public class ctor_System_Diagnostics_EventLogPermission_System_Diagnostics_EventLogPermissionAccess_System_String
{
public static bool _ctor_System_Diagnostics_EventLogPermission_System_Diagnostics_EventLogPermissionAccess_System_String( )
{
   //Parameters
   System.Diagnostics.EventLogPermissionAccess permissionAccess = null;
   System.String machineName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogPermission.ctor(permissionAccess,machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogPermission.ctor(permissionAccess,machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
