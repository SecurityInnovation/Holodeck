namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceBase
{
public class Run_System_ServiceProcess_ServiceBase
{
public static bool _Run_System_ServiceProcess_ServiceBase( )
{
   //Parameters
   System.ServiceProcess.ServiceBase service = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceBase.Run(service);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceBase.Run(service);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
