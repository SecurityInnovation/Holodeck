using System;
using System.Windows.Forms;
using Office = Microsoft.Office.Core;
using Excel = Microsoft.Office.Interop.Excel;
using MSForms = Microsoft.Vbe.Interop.Forms;
using System.IO;
using System.Collections;
using System.Messaging;
using Microsoft.Win32;

///
/// BUGBUG: This code is tightly bound to the StressTester classes which produce the log files. Any change
///         to the log file format will result in failure in this code. 
///

// Office integration attribute. Identifies the startup class for the workbook. Do not modify.
[assembly:System.ComponentModel.DescriptionAttribute("OfficeStartupClass, Version=1.0, Class=ExcelDocumentCreator.OfficeCodeBehind")]

namespace ExcelDocumentCreator
{
    /// <summary>
    /// Contains managed code extensions for the workbook.
    /// </summary>
    public class OfficeCodeBehind
    {
        
        /// <summary>
        /// Application object.
        /// </summary>
        internal Excel.Application ThisApplication
        {
            get { return thisApplication;}
        }

        /// <summary>
        /// Workbook object.
        /// </summary>
        internal Excel.Workbook ThisWorkbook
        {
            get { return thisWorkbook;}
        }

        private Excel.Application thisApplication = null;
        private Excel.Workbook thisWorkbook = null;

        private Excel.WorkbookEvents_OpenEventHandler openEvent;
        private Excel.WorkbookEvents_BeforeCloseEventHandler beforeCloseEvent;
        private Excel.Worksheet testWorkSheet;
        private Excel.Worksheet testChartWorkSheet;
        private int rows = 0;
        
        #region Generated initialization code

        /// <summary>
        /// Default constructor.
        /// </summary>
        public OfficeCodeBehind()
        {
        }

        /// <summary>
        /// Required procedure. Do not modify.
        /// </summary>
        /// <param name="application">Application object.</param>
        /// <param name="workbook">Workbook object.</param>
        public void _Startup(object application, object workbook)
        {
            this.thisApplication = application as Excel.Application;
            this.thisWorkbook = workbook as Excel.Workbook;

            openEvent= new Excel.WorkbookEvents_OpenEventHandler (ThisWorkbook_Open);
            thisWorkbook.Open += openEvent;
            
            beforeCloseEvent = new Excel.WorkbookEvents_BeforeCloseEventHandler(ThisWorkbook_BeforeClose);
            thisWorkbook.BeforeClose += beforeCloseEvent;
        }

        /// <summary>
        /// Required procedure. Do not modify.
        /// </summary>
        public void _Shutdown()
        {
            thisApplication = null;
            thisWorkbook = null;
        }

        /// <summary>
        /// Finds the control with the specified name in the active worksheet.
        /// </summary>
        /// <param name='name'>Name of the control to find.</param>
        /// <returns>
        /// Returns the specified control, or null if it is not found.
        /// </returns>
        object FindControl(string name )
        {
            return FindControl(name, (Excel.Worksheet) ThisWorkbook.ActiveSheet);
        }

        /// <summary>
        /// Returns the control with the specified name in the specified worksheet.
        /// </summary>
        /// <param name='name'>Name of the control to find.</param>
        /// <param name='sheet'>Worksheet object that contains the control.</param>
        /// <returns>
        /// Returns the specified control, or null if it is not found.
        /// </returns>
        object FindControl(string name, Excel.Worksheet sheet ) 
        {
            Excel.OLEObject theObject;
            try
            {
                theObject = (Excel.OLEObject) sheet.OLEObjects(name);
                return theObject.Object;
            }
            catch
            {
                // Returns null if the control is not found.
            }
            return null;
        }

        #endregion

        /// <summary>
        /// Retrieves the worksheet number specified from the workbook...
        /// </summary>
        /// <param name="workbook"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        static public Excel.Worksheet GetWorksheet(Excel.Workbook workbook,int index)
        {
            return workbook.Worksheets[index] as Excel.Worksheet;
        }

        protected void FillExcelSpreadSheet( string logFile )
        {
            StreamReader reader = new StreamReader( logFile );

            string line = null;
            string temp = null;

            ArrayList spreadSheetRow = new ArrayList( );
            int row = 1;
            int column = 1;            
            int TotalProcTimeIndex = 0;
            int PhysMemUsageIndex = 0;
            int VirtMemUsageIndex = 0;
            int ThreadCountIndex = 0;
            int HandleCountIndex = 0;

            // Read the file one line at a time...
            for (;;)
            {
                try
                {
                    line = reader.ReadLine( );

                    if ( line == null )
                        break;
                }
                catch
                {
                    break;
                }

                if ( line.Length != 0 )
                {
                    // If the first character is not equal to an "=", "T", or "E" character, we have a log entry...
                    if ( ( line.ToCharArray( )[0] != '=' ) && ( line.ToCharArray( )[0] != 'T' ) && ( line.ToCharArray( )[0] != 'E' ) && ( line != "\t" ) )
                    {
                        // use index of to find the indexes of the labels... then you can use the sizeof the text at those
                        // labels to determine where the interesting data begins. Then use the beginning (index) of the next
                        // label to determine where it ends...                        
                        TotalProcTimeIndex = line.IndexOf( " TOTAL PROC TIME:" );   // 17 chars.
                        PhysMemUsageIndex = line.IndexOf( " PHYS MEM USAGE:" );     // 16 chars.
                        VirtMemUsageIndex = line.IndexOf( " VIRTUAL MEM USAGE:" );  // 19 chars.
                        ThreadCountIndex = line.IndexOf( " THREAD COUNT:" );        // 14 chars.
                        HandleCountIndex = line.IndexOf( " HANDLE COUNT:" );        // 14 chars.
                                        
                        
                        // We know we have a log entry... buid up an array for the rows to insert into Excel...
                        temp = new String( line.ToCharArray( ) );
                        spreadSheetRow.Add( temp.Split( 'T' )[0].TrimEnd( ' ' ) );      // get date...
                        spreadSheetRow.Add( line.Substring( TotalProcTimeIndex + 17, PhysMemUsageIndex - TotalProcTimeIndex - 17 ) );   // get the proc time...
                        spreadSheetRow.Add( line.Substring( PhysMemUsageIndex + 16, VirtMemUsageIndex - PhysMemUsageIndex - 16 ) );  // get the phys mem usage...
                        spreadSheetRow.Add( line.Substring( VirtMemUsageIndex + 19, ThreadCountIndex - VirtMemUsageIndex - 19  ) ); // get the virtual mem usage... 
                        spreadSheetRow.Add( line.Substring( ThreadCountIndex + 14, HandleCountIndex - ThreadCountIndex - 14 ) ); // get the thread count...                     
                        spreadSheetRow.Add( line.Substring( HandleCountIndex + 14, line.Length - HandleCountIndex - 14 ) ); // get the handle count...

                        // We now have build an array representing row elements... now fill up the excel spreadsheet with
                        // this row...
                        for ( column = 1; column <= spreadSheetRow.Count; column++ )
                        {
                            
                            // The first row is used for column headers, so we do row + 1...
                            ((Excel.Range)this.testWorkSheet.Cells[ row + 1, column ]).Value2 = spreadSheetRow[ column - 1 ];
                        }
                    
                        spreadSheetRow.Clear( );

                        row++;
                    }
                }                
            }

            reader.Close( );

            // store globally accessible row count...
            rows = row;
        }
       
        protected void CreateGraph( int columnIndex )
        {
            // Get the alphabetical column index...
            string columnAlphabetical = ( (char)(columnIndex + 64) ).ToString( );

            // Activate the test Chart worksheet...
            this.testChartWorkSheet.Activate( );

            // Get any charts currently existing in the worksheet...
            Excel.ChartObjects charts = 
                (Excel.ChartObjects)this.testChartWorkSheet.ChartObjects(Type.Missing);

            // Adds a chart at x = 100, y = 300, 500 points wide and 300 tall.
            Excel.ChartObject chartObj = (Excel.ChartObject)charts.Item( columnIndex - 1 );   

            Excel.Chart chart = chartObj.Chart;
                        
            // Gets the data cells ( X values )
            Excel.Range chartRange = this.testWorkSheet.get_Range((columnAlphabetical + "2"), (columnAlphabetical + (rows).ToString( )) );

            chart.SetSourceData(chartRange,Type.Missing);          
                        
            // Set the type of the chart...
            chart.ChartType = Excel.XlChartType.xlLine;                                                
            
            // Get any existing series collection (there should be only one)...
            Excel.SeriesCollection seriesCollection= 
                (Excel.SeriesCollection)chart.SeriesCollection( Type.Missing );
            Excel.Series series = seriesCollection.Item(seriesCollection.Count);

            // Gets the data cells ( Y values )
            series.XValues = this.testWorkSheet.get_Range( "A2", "A" + ( rows ).ToString( ) );
        }

        /// <summary>
        /// Builds the excel document using the specified log file and then saving the excel file to the
        /// output file specified.
        /// </summary>
        /// <param name="logFileInput"></param>
        /// <param name="excelFileOutput"></param>
        protected void BuildExcelDocument( string logFileInput, string excelFileOutput )
        {
            //System.Diagnostics.Debugger.Break( );
            // Get the first worksheet from the workbook...
            this.testWorkSheet = GetWorksheet( this.thisWorkbook, 1 );      
            this.testWorkSheet.Name = "Test Data";

            this.testChartWorkSheet = GetWorksheet( this.thisWorkbook, 2 );
            this.testChartWorkSheet.Name = "Graphs";
            
            // Fill the first spreadsheet with the raw test data...
            FillExcelSpreadSheet( logFileInput );

            CreateGraph( 2 );

            CreateGraph( 3 );

            CreateGraph( 4 );
            
            CreateGraph( 5 );

            CreateGraph( 6 );

            this.thisWorkbook.SaveAs( excelFileOutput,
                Excel.XlFileFormat.xlWorkbookNormal,
                Type.Missing,
                Type.Missing,
                Type.Missing,
                Type.Missing,
                Excel.XlSaveAsAccessMode.xlNoChange,
                Type.Missing,
                Type.Missing,
                Type.Missing,
                Type.Missing,
                Type.Missing );

            /*
            try
            {
                this.thisWorkbook.Close( false, Type.Missing, Type.Missing );
            }
            catch ( Exception )
            {
                // For some reason .NET runtime goes into a stack overflow. This is detected by some other
                // protection features of .NET and it throws an exception. We just catch it here so no
                // errors occur.
            }
            */
                        
        }

        /// <summary>
        /// Get's the location of stress tester and associated documents...
        /// </summary>
        protected string getStressTesterPath( )
        {            
            // Open the localmachine hive...
            RegistryKey localMachine = RegistryKey.OpenRemoteBaseKey( RegistryHive.LocalMachine, System.Environment.MachineName );

            RegistryKey softwareKey = localMachine.OpenSubKey( "SOFTWARE" );

            RegistryKey HDStressKey = softwareKey.OpenSubKey( "HDStress" );

            return HDStressKey.GetValue( "FileLocation" ).ToString( );
        }

        /// <summary>
        /// Called when the workbook is opened.
        /// </summary>
        protected void ThisWorkbook_Open()
        {
            //System.Diagnostics.Debugger.Break( );                       

            // Sleep for a few seconds just to wait for files to close, etc.
            System.Threading.Thread.Sleep( 5000 );

            string stressTesterPath = getStressTesterPath( );

            // First thing we need to do is open up the monitor log file list...
            StreamReader fileList = new StreamReader( stressTesterPath + "\\MONLIST.DAT" );

            string logFileName = fileList.ReadLine( );
            string excelFileName = logFileName + ".xls";

            // Close the file stream...
            fileList.Close( );

            string renamedFileName = null;
            int fileNameCounter = 0;

            // If the file already exists, we rename it...
            if ( File.Exists( excelFileName ) )
            {
                renamedFileName = excelFileName;

                // Figure out a name we can rename the file to...
                while ( File.Exists( renamedFileName ) )
                {                   
                    renamedFileName = excelFileName + "." + fileNameCounter.ToString( );                    

                    fileNameCounter++;
                }
            
                // Now, rename the file...
                File.Move( excelFileName, renamedFileName );
            }

            // Now we can save the new file without causing excel to ask us if we want to overwrite an existing
            // one...
            BuildExcelDocument( logFileName, excelFileName );

            // Code in stresstester will remove the topmost entry in the MONLIST.DAT file so that only the first
            // entry is the one used when excel documents are created...
            
        }

        /// <summary>
        /// Called before the workbook is closed. Note that this method
        /// might be called multiple times and the value assigned to Cancel
        /// might be ignored if other code or the user intervenes.
        /// </summary>
        /// <param name="Cancel">False when the event occurs. If the event procedure
        /// sets this to true, the document does not close when the procedure is finished.
        /// </param>
        protected void ThisWorkbook_BeforeClose(ref bool Cancel)
        {
            Cancel = false;
        }

        public void writeExcelData( )
        {
            ThisWorkbook_Open( );
        }
    }
}
