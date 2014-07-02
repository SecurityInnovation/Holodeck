namespace DotNetInterceptTester.My_System.Net.SocketPermission
{
public class AddPermission_System_Net_SocketPermission_System_Net_NetworkAccess_System_Net_TransportType_System_String_System_Int32
{
public static bool _AddPermission_System_Net_SocketPermission_System_Net_NetworkAccess_System_Net_TransportType_System_String_System_Int32( )
{
   //Parameters
   System.Net.NetworkAccess access = null;
   System.Net.TransportType transport = null;
   System.String hostName = null;
   System.Int32 portNumber = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.SocketPermission.AddPermission(access,transport,hostName,portNumber);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.SocketPermission.AddPermission(access,transport,hostName,portNumber);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
