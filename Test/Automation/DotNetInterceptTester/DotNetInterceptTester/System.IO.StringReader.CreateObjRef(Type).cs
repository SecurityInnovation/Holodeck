namespace DotNetInterceptTester.My_System.IO.StringReader
{
public class CreateObjRef_System_IO_StringReader_System_Type
{
public static bool _CreateObjRef_System_IO_StringReader_System_Type( )
{
   //Parameters
   System.Type requestedType = null;

   //ReturnType/Value
   System.Runtime.Remoting.ObjRef returnVal_Real = null;
   System.Runtime.Remoting.ObjRef returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringReader.CreateObjRef(requestedType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringReader.CreateObjRef(requestedType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
