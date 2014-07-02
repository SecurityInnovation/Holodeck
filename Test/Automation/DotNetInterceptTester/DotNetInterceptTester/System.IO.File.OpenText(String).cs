namespace DotNetInterceptTester.My_System.IO.File
{
public class OpenText_System_String
{
public static bool _OpenText_System_String( )
{
   //Parameters
   System.String path = null;

   //ReturnType/Value
   System.IO.StreamReader returnVal_Real = null;
   System.IO.StreamReader returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.OpenText(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.OpenText(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
