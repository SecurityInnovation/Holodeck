namespace DotNetInterceptTester.My_System.Diagnostics.EventLogPermission
{
public class ToXml_System_Diagnostics_EventLogPermission
{
public static bool _ToXml_System_Diagnostics_EventLogPermission( )
{
   //Parameters

   //ReturnType/Value
   System.Security.SecurityElement returnVal_Real = null;
   System.Security.SecurityElement returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogPermission.ToXml();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogPermission.ToXml();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
