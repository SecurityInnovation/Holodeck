namespace DotNetInterceptTester.My_System.Net.WebResponse
{
public class Close_System_Net_WebResponse
{
public static bool _Close_System_Net_WebResponse( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebResponse.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebResponse.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
