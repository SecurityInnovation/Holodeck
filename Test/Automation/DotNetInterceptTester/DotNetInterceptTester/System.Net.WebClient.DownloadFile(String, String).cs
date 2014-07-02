namespace DotNetInterceptTester.My_System.Net.WebClient
{
public class DownloadFile_System_Net_WebClient_System_String_System_String
{
public static bool _DownloadFile_System_Net_WebClient_System_String_System_String( )
{
   //Parameters
   System.String address = null;
   System.String fileName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebClient.DownloadFile(address,fileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebClient.DownloadFile(address,fileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
