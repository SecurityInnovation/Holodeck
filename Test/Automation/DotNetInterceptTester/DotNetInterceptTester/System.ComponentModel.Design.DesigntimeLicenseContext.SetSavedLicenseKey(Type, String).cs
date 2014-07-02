using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.DesigntimeLicenseContext
{
public class SetSavedLicenseKey_System_ComponentModel_Design_DesigntimeLicenseContext_System_Type_System_String
{
public static bool _SetSavedLicenseKey_System_ComponentModel_Design_DesigntimeLicenseContext_System_Type_System_String( )
{

   //class object
    System.ComponentModel.Design.DesigntimeLicenseContext _System_ComponentModel_Design_DesigntimeLicenseContext = new System.ComponentModel.Design.DesigntimeLicenseContext();

   //Parameters
   System.Type type = null;
   System.String key = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Design_DesigntimeLicenseContext.SetSavedLicenseKey(type,key);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Design_DesigntimeLicenseContext.SetSavedLicenseKey(type,key);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
