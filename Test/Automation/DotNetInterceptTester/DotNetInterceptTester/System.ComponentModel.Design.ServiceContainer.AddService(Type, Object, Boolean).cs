using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.ServiceContainer
{
public class AddService_System_ComponentModel_Design_ServiceContainer_System_Type_System_Object_System_Boolean
{
public static bool _AddService_System_ComponentModel_Design_ServiceContainer_System_Type_System_Object_System_Boolean( )
{

   //class object
    System.ComponentModel.Design.ServiceContainer _System_ComponentModel_Design_ServiceContainer = new System.ComponentModel.Design.ServiceContainer();

   //Parameters
   System.Type serviceType = null;
   System.Object serviceInstance = null;
   System.Boolean promote = false;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Design_ServiceContainer.AddService(serviceType,serviceInstance,promote);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Design_ServiceContainer.AddService(serviceType,serviceInstance,promote);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
