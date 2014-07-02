using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.DesignerTransaction
{
public class Commit_System_ComponentModel_Design_DesignerTransaction
{
public static bool _Commit_System_ComponentModel_Design_DesignerTransaction( )
{

   //class object
    System.ComponentModel.Design.DesignerTransaction _System_ComponentModel_Design_DesignerTransaction = new System.ComponentModel.Design.DesignerTransaction();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_DesignerTransaction.Commit();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_DesignerTransaction.Commit();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
