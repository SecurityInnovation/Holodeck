namespace DotNetInterceptTester.My_System.Net.NetworkCredential
{
public class ctor_System_Net_NetworkCredential_System_String_System_String
{
public static bool _ctor_System_Net_NetworkCredential_System_String_System_String( )
{
   //Parameters
   System.String userName = null;
   System.String password = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.NetworkCredential.ctor(userName,password);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.NetworkCredential.ctor(userName,password);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
