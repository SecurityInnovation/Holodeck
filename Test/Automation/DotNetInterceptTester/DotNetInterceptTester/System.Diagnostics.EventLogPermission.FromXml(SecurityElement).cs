namespace DotNetInterceptTester.My_System.Diagnostics.EventLogPermission
{
public class FromXml_System_Diagnostics_EventLogPermission_System_Security_SecurityElement
{
public static bool _FromXml_System_Diagnostics_EventLogPermission_System_Security_SecurityElement( )
{
   //Parameters
   System.Security.SecurityElement securityElement = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogPermission.FromXml(securityElement);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogPermission.FromXml(securityElement);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
