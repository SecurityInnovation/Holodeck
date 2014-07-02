namespace DotNetInterceptTester.My_System.IO.Directory
{
public class SetLastWriteTime_System_String_System_DateTime
{
public static bool _SetLastWriteTime_System_String_System_DateTime( )
{
   //Parameters
   System.String path = null;
   System.DateTime lastWriteTime = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.Directory.SetLastWriteTime(path,lastWriteTime);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.Directory.SetLastWriteTime(path,lastWriteTime);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
