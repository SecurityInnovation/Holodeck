namespace DotNetInterceptTester.My_System.IO.Directory
{
public class GetFileSystemEntries_System_String
{
public static bool _GetFileSystemEntries_System_String( )
{
   //Parameters
   System.String path = null;

   //ReturnType/Value
   System.String[] returnVal_Real = null;
   System.String[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.Directory.GetFileSystemEntries(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.Directory.GetFileSystemEntries(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
