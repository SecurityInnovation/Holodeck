namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceInstaller
{
public class Dispose_System_ServiceProcess_ServiceInstaller
{
public static bool _Dispose_System_ServiceProcess_ServiceInstaller( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceInstaller.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceInstaller.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
