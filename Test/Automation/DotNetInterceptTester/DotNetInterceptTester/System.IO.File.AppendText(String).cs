namespace DotNetInterceptTester.My_System.IO.File
{
public class AppendText_System_String
{
public static bool _AppendText_System_String( )
{
   //Parameters
   System.String path = null;

   //ReturnType/Value
   System.IO.StreamWriter returnVal_Real = null;
   System.IO.StreamWriter returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.AppendText(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.AppendText(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
