namespace DotNetInterceptTester.My_System.IO.BinaryWriter
{
public class Close_System_IO_BinaryWriter
{
public static bool _Close_System_IO_BinaryWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.BinaryWriter.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.BinaryWriter.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
