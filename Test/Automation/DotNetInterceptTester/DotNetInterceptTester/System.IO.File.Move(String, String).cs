namespace DotNetInterceptTester.My_System.IO.File
{
public class Move_System_String_System_String
{
public static bool _Move_System_String_System_String( )
{
   //Parameters
   System.String sourceFileName = null;
   System.String destFileName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.Move(sourceFileName,destFileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.Move(sourceFileName,destFileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
