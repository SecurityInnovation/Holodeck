namespace DotNetInterceptTester.My_System.IO.TextReader
{
public class Close_System_IO_TextReader
{
public static bool _Close_System_IO_TextReader( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.TextReader.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.TextReader.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
