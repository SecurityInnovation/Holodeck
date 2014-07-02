namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class Select_System_Collections_IList_System_Collections_IList_System_Collections_IList_System_Int32
{
public static bool _Select_System_Collections_IList_System_Collections_IList_System_Collections_IList_System_Int32( )
{
   //Parameters
   System.Collections.IList checkRead = null;
   System.Collections.IList checkWrite = null;
   System.Collections.IList checkError = null;
   System.Int32 microSeconds = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.Select(checkRead,checkWrite,checkError,microSeconds);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.Select(checkRead,checkWrite,checkError,microSeconds);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
