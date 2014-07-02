namespace DotNetInterceptTester.My_System.Diagnostics.Process
{
public class EnterDebugMode
{
public static bool _EnterDebugMode( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Process.EnterDebugMode(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Process.EnterDebugMode(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
