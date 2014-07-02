namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceProcessInstaller
{
public class ctor_System_ServiceProcess_ServiceProcessInstaller
{
public static bool _ctor_System_ServiceProcess_ServiceProcessInstaller( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceProcessInstaller.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceProcessInstaller.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
