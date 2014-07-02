namespace DotNetInterceptTester.My_System.Diagnostics.ProcessModule
{
public class Dispose_System_Diagnostics_ProcessModule
{
public static bool _Dispose_System_Diagnostics_ProcessModule( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.ProcessModule.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.ProcessModule.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
