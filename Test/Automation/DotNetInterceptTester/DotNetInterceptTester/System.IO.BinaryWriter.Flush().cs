namespace DotNetInterceptTester.My_System.IO.BinaryWriter
{
public class Flush_System_IO_BinaryWriter
{
public static bool _Flush_System_IO_BinaryWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.BinaryWriter.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.BinaryWriter.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
