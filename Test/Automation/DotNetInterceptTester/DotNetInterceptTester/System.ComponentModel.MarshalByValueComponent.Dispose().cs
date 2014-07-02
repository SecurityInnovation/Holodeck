namespace DotNetInterceptTester.My_System.ComponentModel.MarshalByValueComponent
{
public class Dispose_System_ComponentModel_MarshalByValueComponent
{
public static bool _Dispose_System_ComponentModel_MarshalByValueComponent( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.MarshalByValueComponent.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.MarshalByValueComponent.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
