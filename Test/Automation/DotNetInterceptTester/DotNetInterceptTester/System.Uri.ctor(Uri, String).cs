namespace DotNetInterceptTester.My_System.Uri
{
public class ctor_System_Uri_System_Uri_System_String
{
public static bool _ctor_System_Uri_System_Uri_System_String( )
{
   //Parameters
   System.Uri baseUri = null;
   System.String relativeUri = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Uri.ctor(baseUri,relativeUri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Uri.ctor(baseUri,relativeUri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
