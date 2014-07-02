namespace DotNetInterceptTester.My_System.Diagnostics.ProcessStartInfo
{
public class ctor_System_Diagnostics_ProcessStartInfo
{
public static bool _ctor_System_Diagnostics_ProcessStartInfo( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.ProcessStartInfo.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.ProcessStartInfo.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
