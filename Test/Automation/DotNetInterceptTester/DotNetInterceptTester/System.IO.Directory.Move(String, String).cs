namespace DotNetInterceptTester.My_System.IO.Directory
{
public class Move_System_String_System_String
{
public static bool _Move_System_String_System_String( )
{
   //Parameters
   System.String sourceDirName = null;
   System.String destDirName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.Directory.Move(sourceDirName,destDirName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.Directory.Move(sourceDirName,destDirName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
