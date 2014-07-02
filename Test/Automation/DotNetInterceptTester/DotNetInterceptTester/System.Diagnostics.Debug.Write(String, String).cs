namespace DotNetInterceptTester.My_System.Diagnostics.Debug
{
public class Write_System_String_System_String
{
public static bool _Write_System_String_System_String( )
{
   //Parameters
   System.String message = null;
   System.String category = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Debug.Write(message,category);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Debug.Write(message,category);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
