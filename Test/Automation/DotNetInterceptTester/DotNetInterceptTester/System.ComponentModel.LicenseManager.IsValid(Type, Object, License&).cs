namespace DotNetInterceptTester.My_System.ComponentModel.LicenseManager
{
public class IsValid_System_Type_System_Object_System_ComponentModel_License_
{
public static bool _IsValid_System_Type_System_Object_System_ComponentModel_License_( )
{
   //Parameters
   System.Type type = null;
   System.Object instance = null;
   System.ComponentModel.License& license = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.ComponentModel.LicenseManager.IsValid(type,instance,license);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.ComponentModel.LicenseManager.IsValid(type,instance,license);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
