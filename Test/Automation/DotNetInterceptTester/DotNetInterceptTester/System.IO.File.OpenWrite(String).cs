namespace DotNetInterceptTester.My_System.IO.File
{
public class OpenWrite_System_String
{
public static bool _OpenWrite_System_String( )
{
   //Parameters
   System.String path = null;

   //ReturnType/Value
   System.IO.FileStream returnVal_Real = null;
   System.IO.FileStream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.OpenWrite(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.OpenWrite(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
