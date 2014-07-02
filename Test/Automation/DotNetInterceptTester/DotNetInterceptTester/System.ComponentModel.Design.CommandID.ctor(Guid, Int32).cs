using System;

namespace DotNetInterceptTester.My_System.ComponentModel.Design.CommandID
{
public class ctor_System_ComponentModel_Design_CommandID_System_Guid_System_Int32
{
public static bool _ctor_System_ComponentModel_Design_CommandID_System_Guid_System_Int32( )
{

   //class object
    System.ComponentModel.Design.CommandID _System_ComponentModel_Design_CommandID = new System.ComponentModel.Design.CommandID();

   //Parameters
   System.Guid menuGroup = null;
   System.Int32 commandID = null;


   //Exception
   System.Exception exception_Real = null;
   System.Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
_System_ComponentModel_Design_CommandID.ctor(menuGroup,commandID);
   }

   catch( System.Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
_System_ComponentModel_Design_CommandID.ctor(menuGroup,commandID);
   }

   catch( System.Exception e )
   {
      exception_Intercepted = e;
   }


   return( ( exception_Real.Message == exception_Intercepted.Message ));
}
}
}
