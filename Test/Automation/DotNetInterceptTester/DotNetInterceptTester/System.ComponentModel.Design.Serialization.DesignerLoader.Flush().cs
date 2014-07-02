using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.Serialization.DesignerLoader
{
public class Flush_System_ComponentModel_Design_Serialization_DesignerLoader
{
public static bool _Flush_System_ComponentModel_Design_Serialization_DesignerLoader( )
{

   //class object
    System.ComponentModel.Design.Serialization.DesignerLoader _System_ComponentModel_Design_Serialization_DesignerLoader = new System.ComponentModel.Design.Serialization.DesignerLoader();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_Design_Serialization_DesignerLoader.Flush();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_Design_Serialization_DesignerLoader.Flush();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
