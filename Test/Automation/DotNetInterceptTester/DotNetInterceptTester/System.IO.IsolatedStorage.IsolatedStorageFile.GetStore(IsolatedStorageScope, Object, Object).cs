namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFile
{
public class GetStore_System_IO_IsolatedStorage_IsolatedStorageScope_System_Object_System_Object
{
public static bool _GetStore_System_IO_IsolatedStorage_IsolatedStorageScope_System_Object_System_Object( )
{
   //Parameters
   System.IO.IsolatedStorage.IsolatedStorageScope scope = null;
   System.Object domainIdentity = null;
   System.Object assemblyIdentity = null;

   //ReturnType/Value
   System.IO.IsolatedStorage.IsolatedStorageFile returnVal_Real = null;
   System.IO.IsolatedStorage.IsolatedStorageFile returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFile.GetStore(scope,domainIdentity,assemblyIdentity);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFile.GetStore(scope,domainIdentity,assemblyIdentity);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
