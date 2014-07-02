using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.DesigntimeLicenseContext
{
public class GetSavedLicenseKey_System_ComponentModel_Design_DesigntimeLicenseContext_System_Type_System_Reflection_Assembly
{
public static bool _GetSavedLicenseKey_System_ComponentModel_Design_DesigntimeLicenseContext_System_Type_System_Reflection_Assembly( )
{

   //class object
    System.ComponentModel.Design.DesigntimeLicenseContext _System_ComponentModel_Design_DesigntimeLicenseContext = new System.ComponentModel.Design.DesigntimeLicenseContext();

   //Parameters
   System.Type type = null;
   System.Reflection.Assembly resourceAssembly = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_Design_DesigntimeLicenseContext.GetSavedLicenseKey(type,resourceAssembly);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_Design_DesigntimeLicenseContext.GetSavedLicenseKey(type,resourceAssembly);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
