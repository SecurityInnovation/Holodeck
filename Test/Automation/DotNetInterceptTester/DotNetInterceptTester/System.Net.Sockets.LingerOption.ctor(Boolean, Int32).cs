namespace DotNetInterceptTester.My_System.Net.Sockets.LingerOption
{
public class ctor_System_Net_Sockets_LingerOption_System_Boolean_System_Int32
{
public static bool _ctor_System_Net_Sockets_LingerOption_System_Boolean_System_Int32( )
{
   //Parameters
   System.Boolean enable = null;
   System.Int32 seconds = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.LingerOption.ctor(enable,seconds);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.LingerOption.ctor(enable,seconds);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
