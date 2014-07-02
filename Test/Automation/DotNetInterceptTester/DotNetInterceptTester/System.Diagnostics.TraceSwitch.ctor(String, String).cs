namespace DotNetInterceptTester.My_System.Diagnostics.TraceSwitch
{
public class ctor_System_Diagnostics_TraceSwitch_System_String_System_String
{
public static bool _ctor_System_Diagnostics_TraceSwitch_System_String_System_String( )
{
   //Parameters
   System.String displayName = null;
   System.String description = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.TraceSwitch.ctor(displayName,description);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.TraceSwitch.ctor(displayName,description);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
