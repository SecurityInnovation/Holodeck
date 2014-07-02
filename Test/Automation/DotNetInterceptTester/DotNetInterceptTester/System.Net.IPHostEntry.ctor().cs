namespace DotNetInterceptTester.My_System.Net.IPHostEntry
{
public class ctor_System_Net_IPHostEntry
{
public static bool _ctor_System_Net_IPHostEntry( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.IPHostEntry.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.IPHostEntry.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
