namespace DotNetInterceptTester.My_System.Diagnostics.Debug
{
public class Assert_System_Boolean_System_String_System_String
{
public static bool _Assert_System_Boolean_System_String_System_String( )
{
   //Parameters
   System.Boolean condition = null;
   System.String message = null;
   System.String detailMessage = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Debug.Assert(condition,message,detailMessage);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Debug.Assert(condition,message,detailMessage);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
