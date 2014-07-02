namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFile
{
public class GetEnumerator_System_IO_IsolatedStorage_IsolatedStorageScope
{
public static bool _GetEnumerator_System_IO_IsolatedStorage_IsolatedStorageScope( )
{
   //Parameters
   System.IO.IsolatedStorage.IsolatedStorageScope scope = null;

   //ReturnType/Value
   System.Collections.IEnumerator returnVal_Real = null;
   System.Collections.IEnumerator returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFile.GetEnumerator(scope);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFile.GetEnumerator(scope);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
