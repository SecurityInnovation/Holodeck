namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceInstaller
{
public class Install_System_ServiceProcess_ServiceInstaller_System_Collections_IDictionary
{
public static bool _Install_System_ServiceProcess_ServiceInstaller_System_Collections_IDictionary( )
{
   //Parameters
   System.Collections.IDictionary stateSaver = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceInstaller.Install(stateSaver);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceInstaller.Install(stateSaver);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
