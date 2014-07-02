namespace DotNetInterceptTester.My_System.IO.File
{
public class GetLastWriteTimeUtc_System_String
{
public static bool _GetLastWriteTimeUtc_System_String( )
{
   //Parameters
   System.String path = null;

   //ReturnType/Value
   System.DateTime returnVal_Real = null;
   System.DateTime returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.GetLastWriteTimeUtc(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.GetLastWriteTimeUtc(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
