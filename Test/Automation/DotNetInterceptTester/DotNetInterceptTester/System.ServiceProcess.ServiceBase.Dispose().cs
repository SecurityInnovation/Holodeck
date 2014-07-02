namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceBase
{
public class Dispose_System_ServiceProcess_ServiceBase
{
public static bool _Dispose_System_ServiceProcess_ServiceBase( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceBase.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceBase.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
