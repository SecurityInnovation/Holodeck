using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.DesigntimeLicenseContext
{
public class GetService_System_ComponentModel_Design_DesigntimeLicenseContext_System_Type
{
public static bool _GetService_System_ComponentModel_Design_DesigntimeLicenseContext_System_Type( )
{

   //class object
    System.ComponentModel.Design.DesigntimeLicenseContext _System_ComponentModel_Design_DesigntimeLicenseContext = new System.ComponentModel.Design.DesigntimeLicenseContext();

   //Parameters
   System.Type type = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_Design_DesigntimeLicenseContext.GetService(type);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_Design_DesigntimeLicenseContext.GetService(type);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
