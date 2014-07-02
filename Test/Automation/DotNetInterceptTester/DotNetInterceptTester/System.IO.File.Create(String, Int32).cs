namespace DotNetInterceptTester.My_System.IO.File
{
public class Create_System_String_System_Int32
{
public static bool _Create_System_String_System_Int32( )
{
   //Parameters
   System.String path = null;
   System.Int32 bufferSize = null;

   //ReturnType/Value
   System.IO.FileStream returnVal_Real = null;
   System.IO.FileStream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.Create(path,bufferSize);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.Create(path,bufferSize);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
