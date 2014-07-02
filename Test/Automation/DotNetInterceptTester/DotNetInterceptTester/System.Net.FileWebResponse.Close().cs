namespace DotNetInterceptTester.My_System.Net.FileWebResponse
{
public class Close_System_Net_FileWebResponse
{
public static bool _Close_System_Net_FileWebResponse( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.FileWebResponse.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.FileWebResponse.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
