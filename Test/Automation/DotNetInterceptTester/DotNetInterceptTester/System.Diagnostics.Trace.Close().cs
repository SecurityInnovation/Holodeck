namespace DotNetInterceptTester.My_System.Diagnostics.Trace
{
public class Close
{
public static bool _Close( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Trace.Close(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Trace.Close(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
