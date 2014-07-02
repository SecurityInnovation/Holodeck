namespace DotNetInterceptTester.My_System.Net.Sockets.NetworkStream
{
public class WriteByte_System_Net_Sockets_NetworkStream_System_Byte
{
public static bool _WriteByte_System_Net_Sockets_NetworkStream_System_Byte( )
{
   //Parameters
   System.Byte _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.NetworkStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.NetworkStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
