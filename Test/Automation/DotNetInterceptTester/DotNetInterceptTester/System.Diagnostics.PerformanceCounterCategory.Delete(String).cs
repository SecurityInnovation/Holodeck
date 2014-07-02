namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class Delete_System_String
{
public static bool _Delete_System_String( )
{
   //Parameters
   System.String categoryName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.Delete(categoryName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.Delete(categoryName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
