namespace DotNetInterceptTester.My_System.IO.FileSystemInfo
{
public class GetObjectData_System_IO_FileSystemInfo_System_Runtime_Serialization_SerializationInfo_System_Runtime_Serialization_StreamingContext
{
public static bool _GetObjectData_System_IO_FileSystemInfo_System_Runtime_Serialization_SerializationInfo_System_Runtime_Serialization_StreamingContext( )
{
   //Parameters
   System.Runtime.Serialization.SerializationInfo info = null;
   System.Runtime.Serialization.StreamingContext context = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileSystemInfo.GetObjectData(info,context);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileSystemInfo.GetObjectData(info,context);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
