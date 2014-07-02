namespace DotNetInterceptTester.My_System.Timers.Timer
{
public class ctor_System_Timers_Timer_System_Double
{
public static bool _ctor_System_Timers_Timer_System_Double( )
{
   //Parameters
   System.Double interval = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Timers.Timer.ctor(interval);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Timers.Timer.ctor(interval);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
