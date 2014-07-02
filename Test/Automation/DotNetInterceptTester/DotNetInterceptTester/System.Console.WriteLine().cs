namespace DotNetInterceptTester.My_System.Console
{
public class WriteLine
{
public static bool _WriteLine( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.WriteLine(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.WriteLine(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
