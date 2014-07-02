namespace DotNetInterceptTester.My_System.Console
{
public class Write_System_UInt64
{
public static bool _Write_System_UInt64( )
{
   //Parameters
   System.UInt64 _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.Write(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.Write(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
