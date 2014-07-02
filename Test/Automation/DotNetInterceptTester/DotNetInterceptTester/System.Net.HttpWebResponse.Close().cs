namespace DotNetInterceptTester.My_System.Net.HttpWebResponse
{
public class Close_System_Net_HttpWebResponse
{
public static bool _Close_System_Net_HttpWebResponse( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.HttpWebResponse.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.HttpWebResponse.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
