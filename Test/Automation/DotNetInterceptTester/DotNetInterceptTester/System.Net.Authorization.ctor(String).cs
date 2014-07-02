namespace DotNetInterceptTester.My_System.Net.Authorization
{
public class ctor_System_Net_Authorization_System_String
{
public static bool _ctor_System_Net_Authorization_System_String( )
{
   //Parameters
   System.String token = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Authorization.ctor(token);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Authorization.ctor(token);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
