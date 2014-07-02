namespace DotNetInterceptTester.My_System.Net.Cookie
{
public class ctor_System_Net_Cookie_System_String_System_String
{
public static bool _ctor_System_Net_Cookie_System_String_System_String( )
{
   //Parameters
   System.String name = null;
   System.String _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Cookie.ctor(name,_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Cookie.ctor(name,_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
