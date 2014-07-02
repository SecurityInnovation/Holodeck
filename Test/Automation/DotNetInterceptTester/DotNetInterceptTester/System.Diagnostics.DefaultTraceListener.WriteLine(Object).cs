namespace DotNetInterceptTester.My_System.Diagnostics.DefaultTraceListener
{
public class WriteLine_System_Diagnostics_DefaultTraceListener_System_Object
{
public static bool _WriteLine_System_Diagnostics_DefaultTraceListener_System_Object( )
{
   //Parameters
   System.Object o = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.DefaultTraceListener.WriteLine(o);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.DefaultTraceListener.WriteLine(o);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
