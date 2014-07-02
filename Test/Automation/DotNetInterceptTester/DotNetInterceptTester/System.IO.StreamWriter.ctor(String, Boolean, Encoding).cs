namespace DotNetInterceptTester.My_System.IO.StreamWriter
{
public class ctor_System_IO_StreamWriter_System_String_System_Boolean_System_Text_Encoding
{
public static bool _ctor_System_IO_StreamWriter_System_String_System_Boolean_System_Text_Encoding( )
{
   //Parameters
   System.String path = null;
   System.Boolean append = null;
   System.Text.Encoding encoding = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamWriter.ctor(path,append,encoding);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamWriter.ctor(path,append,encoding);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
