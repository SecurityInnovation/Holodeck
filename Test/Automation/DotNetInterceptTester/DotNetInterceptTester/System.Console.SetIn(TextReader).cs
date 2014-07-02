namespace DotNetInterceptTester.My_System.Console
{
public class SetIn_System_IO_TextReader
{
public static bool _SetIn_System_IO_TextReader( )
{
   //Parameters
   System.IO.TextReader newIn = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.SetIn(newIn);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.SetIn(newIn);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
