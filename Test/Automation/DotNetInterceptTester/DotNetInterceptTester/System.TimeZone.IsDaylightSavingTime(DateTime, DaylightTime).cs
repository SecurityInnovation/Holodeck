namespace DotNetInterceptTester.My_System.TimeZone
{
public class IsDaylightSavingTime_System_DateTime_System_Globalization_DaylightTime
{
public static bool _IsDaylightSavingTime_System_DateTime_System_Globalization_DaylightTime( )
{
   //Parameters
   System.DateTime time = null;
   System.Globalization.DaylightTime daylightTimes = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.TimeZone.IsDaylightSavingTime(time,daylightTimes);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.TimeZone.IsDaylightSavingTime(time,daylightTimes);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
