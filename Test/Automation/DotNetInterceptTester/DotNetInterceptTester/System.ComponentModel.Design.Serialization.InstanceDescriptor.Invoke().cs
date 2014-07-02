using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.Serialization.InstanceDescriptor
{
public class Invoke_System_ComponentModel_Design_Serialization_InstanceDescriptor
{
public static bool _Invoke_System_ComponentModel_Design_Serialization_InstanceDescriptor( )
{

   //class object
    System.ComponentModel.Design.Serialization.InstanceDescriptor _System_ComponentModel_Design_Serialization_InstanceDescriptor = new System.ComponentModel.Design.Serialization.InstanceDescriptor();

   //Parameters

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnVal_Real = _System_ComponentModel_Design_Serialization_InstanceDescriptor.Invoke();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnVal_Intercepted = _System_ComponentModel_Design_Serialization_InstanceDescriptor.Invoke();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
