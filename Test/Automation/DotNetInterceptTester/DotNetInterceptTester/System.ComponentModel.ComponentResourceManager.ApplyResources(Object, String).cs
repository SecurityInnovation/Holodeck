using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentResourceManager
{
public class ApplyResources_System_ComponentModel_ComponentResourceManager_System_Object_System_String
{
public static bool _ApplyResources_System_ComponentModel_ComponentResourceManager_System_Object_System_String( )
{

   //class object
    System.ComponentModel.ComponentResourceManager _System_ComponentModel_ComponentResourceManager = new System.ComponentModel.ComponentResourceManager();

   //Parameters
   System.Object _value = null;
   System.String objectName = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_ComponentResourceManager.ApplyResources(_value,objectName);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_ComponentResourceManager.ApplyResources(_value,objectName);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
