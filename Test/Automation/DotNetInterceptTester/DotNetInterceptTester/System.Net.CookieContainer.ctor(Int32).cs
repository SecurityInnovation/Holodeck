namespace DotNetInterceptTester.My_System.Net.CookieContainer
{
public class ctor_System_Net_CookieContainer_System_Int32
{
public static bool _ctor_System_Net_CookieContainer_System_Int32( )
{
   //Parameters
   System.Int32 capacity = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CookieContainer.ctor(capacity);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CookieContainer.ctor(capacity);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
