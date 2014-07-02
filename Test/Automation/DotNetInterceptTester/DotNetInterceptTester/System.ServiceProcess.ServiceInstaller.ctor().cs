namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceInstaller
{
public class ctor_System_ServiceProcess_ServiceInstaller
{
public static bool _ctor_System_ServiceProcess_ServiceInstaller( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceInstaller.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceInstaller.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
