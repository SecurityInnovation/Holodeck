namespace DotNetInterceptTester.My_System.IO.File
{
public class SetLastAccessTimeUtc_System_String_System_DateTime
{
public static bool _SetLastAccessTimeUtc_System_String_System_DateTime( )
{
   //Parameters
   System.String path = null;
   System.DateTime lastAccessTimeUtc = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.SetLastAccessTimeUtc(path,lastAccessTimeUtc);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.SetLastAccessTimeUtc(path,lastAccessTimeUtc);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
