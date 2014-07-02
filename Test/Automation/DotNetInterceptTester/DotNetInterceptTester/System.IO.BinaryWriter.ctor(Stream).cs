namespace DotNetInterceptTester.My_System.IO.BinaryWriter
{
public class ctor_System_IO_BinaryWriter_System_IO_Stream
{
public static bool _ctor_System_IO_BinaryWriter_System_IO_Stream( )
{
   //Parameters
   System.IO.Stream output = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.BinaryWriter.ctor(output);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.BinaryWriter.ctor(output);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
