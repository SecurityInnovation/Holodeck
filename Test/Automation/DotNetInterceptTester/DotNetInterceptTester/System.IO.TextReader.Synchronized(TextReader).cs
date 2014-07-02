namespace DotNetInterceptTester.My_System.IO.TextReader
{
public class Synchronized_System_IO_TextReader
{
public static bool _Synchronized_System_IO_TextReader( )
{
   //Parameters
   System.IO.TextReader reader = null;

   //ReturnType/Value
   System.IO.TextReader returnVal_Real = null;
   System.IO.TextReader returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.TextReader.Synchronized(reader);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.TextReader.Synchronized(reader);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
