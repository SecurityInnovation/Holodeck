namespace DotNetInterceptTester.My_System.Timers.Timer
{
public class Stop_System_Timers_Timer
{
public static bool _Stop_System_Timers_Timer( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Timers.Timer.Stop();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Timers.Timer.Stop();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
