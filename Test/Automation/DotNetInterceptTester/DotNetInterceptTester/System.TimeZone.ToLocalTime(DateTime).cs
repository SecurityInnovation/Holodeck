namespace DotNetInterceptTester.My_System.TimeZone
{
public class ToLocalTime_System_TimeZone_System_DateTime
{
public static bool _ToLocalTime_System_TimeZone_System_DateTime( )
{
   //Parameters
   System.DateTime time = null;

   //ReturnType/Value
   System.DateTime returnVal_Real = null;
   System.DateTime returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.TimeZone.ToLocalTime(time);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.TimeZone.ToLocalTime(time);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
