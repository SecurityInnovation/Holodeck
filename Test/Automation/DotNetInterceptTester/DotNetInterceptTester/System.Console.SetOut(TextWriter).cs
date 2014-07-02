namespace DotNetInterceptTester.My_System.Console
{
public class SetOut_System_IO_TextWriter
{
public static bool _SetOut_System_IO_TextWriter( )
{
   //Parameters
   System.IO.TextWriter newOut = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.SetOut(newOut);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.SetOut(newOut);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
