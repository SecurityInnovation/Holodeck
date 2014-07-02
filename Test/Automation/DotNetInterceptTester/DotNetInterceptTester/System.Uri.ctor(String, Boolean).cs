namespace DotNetInterceptTester.My_System.Uri
{
public class ctor_System_Uri_System_String_System_Boolean
{
public static bool _ctor_System_Uri_System_String_System_Boolean( )
{
   //Parameters
   System.String uriString = null;
   System.Boolean dontEscape = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Uri.ctor(uriString,dontEscape);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Uri.ctor(uriString,dontEscape);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
