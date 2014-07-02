namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class CreateObjRef_System_Diagnostics_EventLog_System_Type
{
public static bool _CreateObjRef_System_Diagnostics_EventLog_System_Type( )
{
   //Parameters
   System.Type requestedType = null;

   //ReturnType/Value
   System.Runtime.Remoting.ObjRef returnVal_Real = null;
   System.Runtime.Remoting.ObjRef returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.CreateObjRef(requestedType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.CreateObjRef(requestedType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
