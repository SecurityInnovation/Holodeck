namespace DotNetInterceptTester.My_System.ServiceProcess.ServiceInstaller
{
public class Commit_System_ServiceProcess_ServiceInstaller_System_Collections_IDictionary
{
public static bool _Commit_System_ServiceProcess_ServiceInstaller_System_Collections_IDictionary( )
{
   //Parameters
   System.Collections.IDictionary savedState = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ServiceProcess.ServiceInstaller.Commit(savedState);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ServiceProcess.ServiceInstaller.Commit(savedState);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
