namespace DotNetInterceptTester.My_System.Diagnostics.Debug
{
public class WriteLine_System_Object
{
public static bool _WriteLine_System_Object( )
{
   //Parameters
   System.Object _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.Debug.WriteLine(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.Debug.WriteLine(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
