namespace BrokenApp
{
   public __gc class ourDate {
   public:
      ourDate(String * str)
      {
         //take string
         dateStr=str;
      }
      ourDate(void)
      {
         //do nothing
      }
      String * toString()
      {
         return dateStr;
      }
   private: String * dateStr;
   };
}