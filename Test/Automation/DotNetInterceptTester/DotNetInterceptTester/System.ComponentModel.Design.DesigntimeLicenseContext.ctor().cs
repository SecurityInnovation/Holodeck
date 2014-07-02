using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.DesigntimeLicenseContext
{
public class ctor_System_ComponentModel_Design_DesigntimeLicenseContext
{
public static bool _ctor_System_ComponentModel_Design_DesigntimeLicenseContext( )
{

   //class object
    System.ComponentModel.Design.DesigntimeLicenseContext _System_ComponentModel_Design_DesigntimeLicenseContext = new System.ComponentModel.Design.DesigntimeLicenseContext();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_DesigntimeLicenseContext.ctor();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_DesigntimeLicenseContext.ctor();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
