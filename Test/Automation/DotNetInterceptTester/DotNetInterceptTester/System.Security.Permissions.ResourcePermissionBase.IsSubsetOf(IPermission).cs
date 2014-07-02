namespace DotNetInterceptTester.My_System.Security.Permissions.ResourcePermissionBase
{
public class IsSubsetOf_System_Security_Permissions_ResourcePermissionBase_System_Security_IPermission
{
public static bool _IsSubsetOf_System_Security_Permissions_ResourcePermissionBase_System_Security_IPermission( )
{
   //Parameters
   System.Security.IPermission target = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Permissions.ResourcePermissionBase.IsSubsetOf(target);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Permissions.ResourcePermissionBase.IsSubsetOf(target);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
