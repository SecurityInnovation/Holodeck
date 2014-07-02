namespace DotNetInterceptTester.My_System.Console
{
public class WriteLine_System_Int32
{
public static bool _WriteLine_System_Int32( )
{
   //Parameters
   System.Int32 _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.WriteLine(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.WriteLine(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
