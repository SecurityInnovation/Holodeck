using System;

namespace DotNetInterceptTester.My_System.ComponentModel.ComponentResourceManager
{
public class ApplyResources_System_ComponentModel_ComponentResourceManager_System_Object_System_String_System_Globalization_CultureInfo
{
public static bool _ApplyResources_System_ComponentModel_ComponentResourceManager_System_Object_System_String_System_Globalization_CultureInfo( )
{

   //class object
    System.ComponentModel.ComponentResourceManager _System_ComponentModel_ComponentResourceManager = new System.ComponentModel.ComponentResourceManager();

   //Parameters
   System.Object _value = null;
   System.String objectName = null;
   System.Globalization.CultureInfo culture = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_ComponentResourceManager.ApplyResources(_value,objectName,culture);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_ComponentResourceManager.ApplyResources(_value,objectName,culture);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
