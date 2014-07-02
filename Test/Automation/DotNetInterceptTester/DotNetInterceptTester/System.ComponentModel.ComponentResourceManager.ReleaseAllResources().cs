using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentResourceManager
{
public class ReleaseAllResources_System_ComponentModel_ComponentResourceManager
{
public static bool _ReleaseAllResources_System_ComponentModel_ComponentResourceManager( )
{

   //class object
    System.ComponentModel.ComponentResourceManager _System_ComponentModel_ComponentResourceManager = new System.ComponentModel.ComponentResourceManager();

   //Parameters


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      _System_ComponentModel_ComponentResourceManager.ReleaseAllResources();
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      _System_ComponentModel_ComponentResourceManager.ReleaseAllResources();
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
