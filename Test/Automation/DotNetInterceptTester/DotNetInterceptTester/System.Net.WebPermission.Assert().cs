namespace DotNetInterceptTester.My_System.Net.WebPermission
{
public class Assert_System_Net_WebPermission
{
public static bool _Assert_System_Net_WebPermission( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebPermission.Assert();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebPermission.Assert();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
