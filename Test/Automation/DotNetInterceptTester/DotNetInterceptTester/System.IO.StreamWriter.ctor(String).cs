namespace DotNetInterceptTester.My_System.IO.StreamWriter
{
public class ctor_System_IO_StreamWriter_System_String
{
public static bool _ctor_System_IO_StreamWriter_System_String( )
{
   //Parameters
   System.String path = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamWriter.ctor(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamWriter.ctor(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
