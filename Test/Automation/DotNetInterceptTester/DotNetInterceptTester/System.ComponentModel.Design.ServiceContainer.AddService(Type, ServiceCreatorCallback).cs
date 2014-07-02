using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.ServiceContainer
{
public class AddService_System_ComponentModel_Design_ServiceContainer_System_Type_System_ComponentModel_Design_ServiceCreatorCallback
{
public static bool _AddService_System_ComponentModel_Design_ServiceContainer_System_Type_System_ComponentModel_Design_ServiceCreatorCallback( )
{

   //class object
    System.ComponentModel.Design.ServiceContainer _System_ComponentModel_Design_ServiceContainer = new System.ComponentModel.Design.ServiceContainer();

   //Parameters
   System.Type serviceType = null;
   System.ComponentModel.Design.ServiceCreatorCallback callback = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Design_ServiceContainer.AddService(serviceType,callback);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Design_ServiceContainer.AddService(serviceType,callback);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
