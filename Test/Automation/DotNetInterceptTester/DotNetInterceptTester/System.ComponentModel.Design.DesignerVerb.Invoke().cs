using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.DesignerVerb
{
public class Invoke_System_ComponentModel_Design_DesignerVerb
{
public static bool _Invoke_System_ComponentModel_Design_DesignerVerb( )
{

   //class object
    System.ComponentModel.Design.DesignerVerb _System_ComponentModel_Design_DesignerVerb = new System.ComponentModel.Design.DesignerVerb();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_DesignerVerb.Invoke();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_DesignerVerb.Invoke();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
