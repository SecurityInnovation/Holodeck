namespace DotNetInterceptTester.My_System.Console
{
public class SetError_System_IO_TextWriter
{
public static bool _SetError_System_IO_TextWriter( )
{
   //Parameters
   System.IO.TextWriter newError = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Console.SetError(newError);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Console.SetError(newError);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
