namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceProcessInstaller
{
public class Dispose_System_ServiceProcess_ServiceProcessInstaller
{
public static bool _Dispose_System_ServiceProcess_ServiceProcessInstaller( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceProcessInstaller.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceProcessInstaller.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
