using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.HtmlControls;
using Database;
using Microsoft.Web.UI.WebControls;

namespace testtracker
{
	/// <summary>
	/// Summary description for WebForm1.
	/// </summary>
    public class TTWebForm : System.Web.UI.Page
    {
        protected System.Web.UI.WebControls.Panel TitlePanel;
        protected System.Web.UI.WebControls.Panel UserPanel;
        protected System.Web.UI.WebControls.Label UserNameLabel;
        protected Microsoft.Web.UI.WebControls.TreeView TestTreeView;
        public static string userName = null;
        public SQLDatabase dbConnection;
        protected string DBUserName = "testtracker";
        protected string DBPassword = "#testtracker#";
        protected string SQLServer = "teejay2.se.fit.edu";
        protected string TreeViewImageUrl = "./images/";
        protected string RootImage = "root.gif";
        protected string ChildAreaImage = "folder.gif";
        protected System.Web.UI.WebControls.Panel AreaInfoPanel;
        protected string TestImage = "litening.gif";
        protected System.Web.UI.WebControls.Label NumberOfTestsInAreaValue;
        protected System.Web.UI.WebControls.DataGrid AreaTestsGrid;
        protected System.Web.UI.WebControls.Panel TestEditPanel;
        protected System.Web.UI.WebControls.Button Button1;
        protected System.Web.UI.WebControls.TextBox EditTestTestName;
        protected System.Web.UI.WebControls.TextBox EditTestTestDescription;
        protected System.Web.UI.WebControls.ListBox EditTestTestPriorityListBox;
        protected System.Web.UI.WebControls.TextBox EditTestExecutionPath;
        protected System.Web.UI.WebControls.TextBox EditTestExecutionName;
        protected System.Web.UI.WebControls.TextBox EditTestExecutionArgs;
        protected System.Web.UI.WebControls.TextBox EditTestState;
        protected System.Web.UI.WebControls.TextBox RunFreqMonths;
        protected System.Web.UI.WebControls.TextBox RunFreqDays;
        protected System.Web.UI.WebControls.TextBox RunFreqHours;
        protected System.Web.UI.WebControls.TextBox RunFreqMinutes;
        protected System.Web.UI.WebControls.DropDownList EditTestOS;
        protected System.Web.UI.WebControls.DropDownList EditTestOSVersion;
        protected System.Web.UI.WebControls.Label SelectAnAreaOrTest;
        protected System.Web.UI.WebControls.Button DeleteArea;
        protected System.Web.UI.WebControls.Button AddArea;
        protected System.Web.UI.WebControls.Label ErrorMessage;
        protected System.Web.UI.WebControls.TextBox AreaNameToAdd;
        protected System.Web.UI.WebControls.Button AddNewTestButton;
        protected System.Web.UI.WebControls.Label TestEditLabel;
        protected System.Web.UI.WebControls.Label AreaSelectedValue;
        protected System.Web.UI.WebControls.Label AreaSelectedTitle;
        protected System.Web.UI.WebControls.DropDownList EditTestOwnerList;
        protected System.Web.UI.WebControls.Label EditTestInternalID;
        protected System.Web.UI.WebControls.RadioButton EditTestInfiniteNoCare;
        protected System.Web.UI.WebControls.RadioButton EditTestInfinite;
        protected System.Web.UI.WebControls.TextBox EditTestRunNumberOfTimes;
        protected System.Web.UI.WebControls.Label PriorityRequiredError;
        protected System.Web.UI.WebControls.Label AreaHolder;
        protected System.Web.UI.WebControls.DataGrid TestHistoryGrid;
        protected System.Web.UI.WebControls.Panel HistoryPanel;
        protected ArrayList ExpandedNodes;
    
        /// <summary>
        /// Finds the specified node in the tree one level deep.
        /// </summary>
        /// <param name="nodeID"></param>
        /// <returns></returns>
        private TreeNode FindFirstLevelNodeInTree( string nodeID )
        {
            foreach ( TreeNode node in TestTreeView.Nodes )
            {
                if ( node.ID == nodeID )
                    return node;
            }

            return null;
        }

        /// <summary>
        /// Searches the entire tree recursively for the specified node.
        /// </summary>
        /// <param name="nodeID"></param>
        /// <returns></returns>
        private TreeNode FindNodeInTreeRecursive( string nodeID )
        {
            foreach ( TreeNode node in TestTreeView.Nodes )
            {
                if ( node.ID == nodeID )
                    return node;
                else
                    return FindNodeInTreeRecursive( nodeID );
            }

            return null;
        }

        /// <summary>
        /// Queries the database for tests belonging to the area and then adds
        /// the tests as nodes in the tree.
        /// </summary>
        /// <param name="node"></param>
        private void AddTestsToAreaNode ( TreeNode node )
        {
            ArrayList testIDs = null;
            Test testEntry = null;
            TreeNode testNode = null;
            
            testIDs = dbConnection.RetrieveTestIDsInArea( node.ID );

            foreach ( string test in testIDs )
            {
                testEntry = dbConnection.RetrieveTest( test );

                testNode = new TreeNode( );
                testNode.ID = testEntry.ID;
                testNode.Text = testEntry.Name;
                testNode.Type = "Test";
                testNode.ImageUrl = TreeViewImageUrl + TestImage;

                node.Nodes.Add( testNode );
            }
        }

        /// <summary>
        /// Populates the entire tree starting at the parent ID.
        /// </summary>
        /// <param name="parentID"></param>
        private void PopulateChildren( string parentID, TreeNode lastNode )
        {
            ArrayList childNodeIDs = null;
            Area areaEntry = null;
            TreeNode parentNode = null;

            childNodeIDs = dbConnection.RetrieveChildAreaIDs( parentID );

            if ( lastNode == null )
                parentNode = FindFirstLevelNodeInTree( parentID );
            else
                parentNode = lastNode;

            // If there are no nodes, just return and don't do anything...
            if ( parentNode == null )
                return;

            // Add tests to the parent node if they exist...
            AddTestsToAreaNode( parentNode );

            foreach ( string nodeID in childNodeIDs )
            {
                areaEntry = dbConnection.RetrieveArea( nodeID );

                if ( ( areaEntry.ID != areaEntry.Parent ) && ( parentNode != null ) )
                {                    
                    TreeNode newChild = new TreeNode( );
                    newChild.ID = areaEntry.ID;
                    newChild.Text = areaEntry.Name;
                    newChild.Type = "Area";
                    newChild.ImageUrl = TreeViewImageUrl + ChildAreaImage;

                    // Add the child to the tree...
                    parentNode.Nodes.Add( newChild ); 
                                                   
                    // Recusively populate children here next...                
                    PopulateChildren( areaEntry.ID, newChild );
                }
            }           
        }

        private void PopulateTreeview( )
        {
            TreeNode treeNode = new TreeNode( );                       
         
            // First, let's clear all the nodes in the tree-view out...
            TestTreeView.Nodes.Clear( );

            // Get the root node...
            Area rootArea = dbConnection.RetrieveArea( dbConnection.RetreiveRootAreaID( ) );

            // Add the root node to the tree...
            treeNode.Text = rootArea.Name;
            treeNode.ID = rootArea.ID;
            treeNode.Type = "Area";
            treeNode.ImageUrl = TreeViewImageUrl + RootImage;

            TestTreeView.Nodes.Add( treeNode );

            // Populate all children underneath this root node.
            PopulateChildren( rootArea.ID, null );
        }

        private void Page_Load(object sender, System.EventArgs e)
        {
            dbConnection = new SQLDatabase( );

            // The expanded nodes arraylist tracks the expanded and collapsed nodes
            // of the tree view.
            ExpandedNodes = new ArrayList( );
            
            // We must maintain the expanded nodes across the session...
            if ( Session["ExpandedNodes"] == null )           
                Session["ExpandedNodes"] = ExpandedNodes;          

            dbConnection.DataSource = "user id=" + DBUserName + ";password=" + DBPassword + ";Data Source=" + SQLServer + ";Initial Catalog=" 
                + dbConnection.InitialCatalog;
            
            // Connect to the database...
            dbConnection.Connect( dbConnection.DataSource );
                        
            // Put user code to initialize the page here
            UserNameLabel.Text = userName;
                        
            Permissions userPermissions = dbConnection.RetrieveUser( dbConnection.GetUserIDFromExternalID( userName ) ).PermissionSettings;

            AddNewTestButton.Enabled = userPermissions.CreateTests;
            DeleteArea.Enabled = userPermissions.DeleteAreas;
            AddArea.Enabled = userPermissions.CreateAreas;
            AreaTestsGrid.Columns[2].Visible = userPermissions.DeleteTests;
            AreaTestsGrid.Columns[0].Visible = userPermissions.ModifyTests;

            this.PopulateTreeview( );
        }

        /// <summary>
        /// Recursively fills a DataTable containing all the tests beneath the parent
        /// area passed in.
        /// </summary>
        /// <param name="areaID"></param>
        private void GetAllChildTestIDsOfArea( string areaID, ref ArrayList testIDs )
        {
            // First, let's look at testIDs in the current area...
            foreach ( string testID in dbConnection.RetrieveTestIDsInArea( areaID ) )
            {
                if ( !testIDs.Contains( testID ) )
                    testIDs.Add( testID );
            }

            // Cycle through all the child areas if any exist...
            foreach ( string childArea in dbConnection.RetrieveChildAreaIDs( areaID ) )
            {               
                foreach ( string testID in dbConnection.RetrieveTestIDsInArea( childArea ) )
                {
                    // If the test ID has not already been added to the list, add it...
                    if ( !testIDs.Contains( testID ) )
                        testIDs.Add( testID );
                }
               
                // Recursively call ourselves again to get remaining test IDs...
                if ( childArea != areaID )  // make sure no loop exists...
                    GetAllChildTestIDsOfArea( childArea, ref testIDs );
            }

            return;
        }

        private void ShowSelectedAreaInformation( TreeNode selectedNode )
        {
            AreaInfoPanel.Visible = true;
            TestEditPanel.Visible = false;
            HistoryPanel.Visible = false;

            AreaSelectedValue.Text = selectedNode.Text;

            ArrayList testIDs = new ArrayList( );

            // Get the child tests associated with the area...
            GetAllChildTestIDsOfArea( selectedNode.ID, ref testIDs );

            // Now fill up a data structure containing Test Classes...
            ArrayList testClasses = new ArrayList( );

            foreach ( string testID in testIDs )
            {
                // Add the test to the array list...
                testClasses.Add( dbConnection.RetrieveTest( testID ) );
            }

            // Show the number of tests in the area...
            NumberOfTestsInAreaValue.Text = testClasses.Count.ToString( );

            DataTable table = new DataTable( );

            // Add the required columns to the data grid...
            table.Columns.Add( new DataColumn( "Name", typeof(String) ) );
            table.Columns.Add( new DataColumn( "Priority", typeof(String) ) );
            table.Columns.Add( new DataColumn( "Owner", typeof(String) ) );
            table.Columns.Add( new DataColumn( "State", typeof(String) ) );
            //table.Columns.Add( new DataColumn( "ID", typeof(String) ) );

            // Go through all the test classes and add the items to the data source...
            foreach ( Test testClass in testClasses )
            {
                table.Rows.Add( new object [] { testClass.Name, 
                                                  testClass.Priority, 
                                                  dbConnection.RetrieveUser( testClass.OwnerID ).Name, 
                                                  testClass.State } ); 
            }                      

            // Now, associate this table of data to the data grid...
            AreaTestsGrid.DataSource = table;
            AreaTestsGrid.DataBind( );          
                
            // Assign IDs to each of the items...
            for ( int count = 0; count < table.Rows.Count; count++ )
            {
                AreaTestsGrid.Items[count].Attributes["ID"] = ((Test)testClasses[count]).ID;
            }
    
        }

        private void ShowSelectedTestInformation( string testID )
        {
            // Make the test panel visible...
            AreaInfoPanel.Visible = false;
            TestEditPanel.Visible = true;
            HistoryPanel.Visible = false;

            TestEditLabel.Text = "Edit/View Test";            

            Test testInformation = dbConnection.RetrieveTest( testID );

            EditTestTestName.Text = testInformation.Name;
            EditTestTestDescription.Text = testInformation.Description;

            // First, clear out the test owners list...
            EditTestOwnerList.Items.Clear( );

            // Populate the owner list...
            foreach ( User user in dbConnection.RetrieveAllUsers( ) )
            {
                EditTestOwnerList.Items.Add( new ListItem( user.Name, user.Name ) );
            }

            // Set the currently selected item to the test owner in the user list...
            EditTestOwnerList.SelectedValue = dbConnection.RetrieveUser( testInformation.OwnerID ).Name;

            // Clear any selections in the priority list box...
            EditTestTestPriorityListBox.ClearSelection( );

            // Now, select the current priority given to the test...
            ListItem priorityItem = EditTestTestPriorityListBox.Items.FindByText( "P" + testInformation.Priority );

            if ( priorityItem != null )
                priorityItem.Selected = true;

            EditTestExecutionPath.Text = testInformation.ExecutionPath;
            EditTestExecutionName.Text = testInformation.ExecutionBinary;
            EditTestExecutionArgs.Text = testInformation.ExecutionArgs;
            
            if ( EditTestOS.Items.FindByValue( testInformation.OperatingSystem ) != null )
                EditTestOS.SelectedValue = testInformation.OperatingSystem;

            if ( EditTestOSVersion.Items.FindByValue( testInformation.OSVersion ) != null )
                EditTestOSVersion.SelectedValue = testInformation.OSVersion;
            
            // Figure out the date and time components of the interval and set them...
            RunFrequency runFrequency = new RunFrequency( testInformation.RunFrequency );
            
            RunFreqMonths.Text = ( runFrequency.Interval.Days / 30 ).ToString( );  // Get number of months...
            RunFreqDays.Text = ( runFrequency.Interval.Days % 30 ).ToString( );    // Get number of days (left from months)...
            RunFreqHours.Text = runFrequency.Interval.Hours.ToString( );
            RunFreqMinutes.Text = runFrequency.Interval.Minutes.ToString( );
            EditTestRunNumberOfTimes.Text = runFrequency.RunNumberOfTimes.ToString( );
            
            if ( runFrequency.RunInfinite )
            {
                EditTestInfinite.Checked = true;
                EditTestInfiniteNoCare.Checked = false;
            }
            else
            {
                EditTestInfiniteNoCare.Checked = true;
                EditTestInfinite.Checked = true;
            }
                 
            EditTestState.Text = testInformation.State;

            EditTestInternalID.Text = testInformation.ID;
        }

        #region Web Form Designer generated code
        override protected void OnInit(EventArgs e)
        {
            //
            // CODEGEN: This call is required by the ASP.NET Web Form Designer.
            //
            InitializeComponent();
            base.OnInit(e);
        }
		
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {    
            this.Button1.Click += new System.EventHandler(this.Button1_Click);
            this.TestTreeView.Load += new System.EventHandler(this.TestTreeView_Load);
            this.TestTreeView.Expand += new Microsoft.Web.UI.WebControls.ClickEventHandler(this.TestTreeView_Expand);
            this.TestTreeView.SelectedIndexChange += new Microsoft.Web.UI.WebControls.SelectEventHandler(this.TestTreeView_SelectedIndexChange);
            this.TestTreeView.Collapse += new Microsoft.Web.UI.WebControls.ClickEventHandler(this.TestTreeView_Collapse);
            this.AddNewTestButton.Click += new System.EventHandler(this.AddNewTestButton_Click);
            this.DeleteArea.Click += new System.EventHandler(this.DeleteArea_Click);
            this.AddArea.Click += new System.EventHandler(this.AddArea_Click);
            this.AreaTestsGrid.ItemCommand += new System.Web.UI.WebControls.DataGridCommandEventHandler(this.AreaTestsGrid_ItemCommand);
            this.AreaTestsGrid.PageIndexChanged += new System.Web.UI.WebControls.DataGridPageChangedEventHandler(this.AreaTestsGrid_PageIndexChanged);
            this.AreaTestsGrid.EditCommand += new System.Web.UI.WebControls.DataGridCommandEventHandler(this.AreaTestsGrid_EditCommand);
            this.AreaTestsGrid.DeleteCommand += new System.Web.UI.WebControls.DataGridCommandEventHandler(this.AreaTestsGrid_DeleteCommand);
            this.Load += new System.EventHandler(this.Page_Load);

        }
        #endregion
  
        
        private void TestTreeView_Load(object sender, System.EventArgs e)
        {
            this.ExpandTreeNodes( );
            this.SetSelectedNode( );
        }
        
        private void SetSelectedNode( )
        {                    
            try
            {
                TestTreeView.SelectedNodeIndex = (string)Session["SelectedNode"];                        
            }
            catch (Exception)
            {
                // Repopulate the tree view if we could not find the selected node...
                this.PopulateTreeview( );

                // Reset session state variables...
                Session["SelectedNode"] = null;
                Session["ExpandedNodes"] = null;                
            }
        }

        private void TestTreeView_SelectedIndexChange(object sender, Microsoft.Web.UI.WebControls.TreeViewSelectEventArgs e)
        {
            // Set the currently selected node...
            Session["SelectedNode"] = e.NewNode;

            // Expand the tree nodes before performing a selection...
            this.ExpandTreeNodes( );

            TreeNode selectedNode = 
                TestTreeView.GetNodeFromIndex( e.NewNode );

            // Need to make sure that the currently selected node really is the
            // selected node...
            this.SetSelectedNode( );

            if ( selectedNode.Type == "Area" )
                this.ShowSelectedAreaInformation( selectedNode );
            else
                this.ShowSelectedTestInformation( selectedNode.ID );            

            // Clear out any visible error messages...
            ErrorMessage.Text = "";
        }

        /// <summary>
        /// Expands nodes that have been requested for expansion by the user.
        /// </summary>
        private void ExpandTreeNodes( )
        {            
            try
            {
                foreach ( string node in ( ((ArrayList)Session["ExpandedNodes"]) ) )
                {                
                    TestTreeView.GetNodeFromIndex( node ).Expanded = true;
                }
            }
            catch ( Exception )
            {
                // If an exception occurs, reset the tree view...
                this.PopulateTreeview( );
                
                // Reset session state variables...
                Session["SelectedNode"] = null;
                Session["ExpandedNodes"] = null;
            }
        }

        private void TestTreeView_Collapse(object sender, Microsoft.Web.UI.WebControls.TreeViewClickEventArgs e)
        {            
            // Remove the collapsed node from the expansion list...
            ((ArrayList)Session["ExpandedNodes"]).Remove( e.Node );
             
            // Collapse the node (for state could be saved)...
            TestTreeView.GetNodeFromIndex( e.Node ).Expanded = false;

            // Expand the nodes that need to be expanded...
            this.ExpandTreeNodes( );

            // Select the node that was previously selected...
            this.SetSelectedNode( );

            // Clear out any visible error messages...
            ErrorMessage.Text = "";
        }

        private void TestTreeView_Expand(object sender, Microsoft.Web.UI.WebControls.TreeViewClickEventArgs e)
        {            
            // Add the newly expanded node to the node array...
            // If the list doesn't contain the node, then add it to the list...
            if ( Session["ExpandedNodes"] == null )
                Session["ExpandedNodes"] = new ArrayList( );            
                
            if ( !((ArrayList)Session["ExpandedNodes"]).Contains( e.Node ) )
                ((ArrayList)Session["ExpandedNodes"]).Add( e.Node );            

            // Expand the nodes that need to be expanded...
            this.ExpandTreeNodes( );          
  
            // Select the node that was previously selected...
            this.SetSelectedNode( );

            // Clear out any visible error messages...
            ErrorMessage.Text = "";
        }

        private void DeleteArea_Click(object sender, System.EventArgs e)
        {
            if ( dbConnection.RetrieveChildAreaIDs( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ).ID ).Count != 0 )
            {
                ErrorMessage.Text = "Cannot Delete while Child Areas Exist!";

                return;
            }

            if ( dbConnection.RetrieveTestIDsInArea( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ).ID ).Count != 0 )
            {
                ErrorMessage.Text = "Cannot Delete while Tests Exist!";

                return;
            }
            
            // If there are no child areas or child tests, then we can continue to perform the deletion...
            dbConnection.DeleteArea( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ).ID );
            
            // Now, we must rebuild the test tree view...
            PopulateTreeview( );

            AreaInfoPanel.Visible = false;
            TestEditPanel.Visible = false;
        }

        private void AddArea_Click(object sender, System.EventArgs e)
        {
            Area area = new Area( );
            area.Parent = TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ).ID;
            area.Name = AreaNameToAdd.Text;

            dbConnection.AddArea( area );

            PopulateTreeview( );

            AreaInfoPanel.Visible = false;
            TestEditPanel.Visible = false;
        }

        /// <summary>
        /// Handles the editing of items in the test list...
        /// </summary>
        /// <param name="source"></param>
        /// <param name="e"></param>
        private void AreaTestsGrid_EditCommand(object source, System.Web.UI.WebControls.DataGridCommandEventArgs e)
        {
            ShowSelectedTestInformation( e.Item.Attributes["ID"] );
        }

        private void AreaTestsGrid_DeleteCommand(object source, System.Web.UI.WebControls.DataGridCommandEventArgs e)
        {
            // Delete the test item...
            dbConnection.DeleteTest( e.Item.Attributes["ID"] );

            // Show the newly updated list of test information...
            ShowSelectedAreaInformation( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ) );

            // Repopulate the tree view...
            PopulateTreeview( );

            this.ExpandTreeNodes( );
            this.SetSelectedNode( );
        }

        private void AddNewTestButton_Click(object sender, System.EventArgs e)
        {
            TestEditPanel.Visible = true;
            TestEditLabel.Text = "Add New Test";

            // Remove items from the list...
            EditTestOwnerList.Items.Clear( );

            foreach ( User user in dbConnection.RetrieveAllUsers( ) )
            {
                EditTestOwnerList.Items.Add( new ListItem( user.Name, user.Name ) );
            }

            EditTestState.Text = "State Cannot Be Set";

            RunFreqMonths.Text = "0";
            RunFreqDays.Text = "1";
            RunFreqHours.Text = "0";
            RunFreqMinutes.Text = "0";
            EditTestRunNumberOfTimes.Text = "0";

            AreaHolder.Text = TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ).ID;
        }

        /// <summary>
        /// Handles the updation and saving of tests...
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Button1_Click(object sender, System.EventArgs e)
        {
            Test test = new Test( );

            // Determine if we are updating or adding a test to the database...
            if ( dbConnection.RetrieveTest( EditTestInternalID.Text ) == null )
            {
                if ( EditTestTestPriorityListBox.SelectedItem == null )
                {
                    PriorityRequiredError.Text = "You must enter a priority.";

                    return;
                }

                // here we are adding a new test...
                test.Name = EditTestTestName.Text;
                test.Description = EditTestTestDescription.Text;
                test.Priority = EditTestTestPriorityListBox.SelectedItem.Text.Replace( "P", "" );
                test.OwnerID = dbConnection.GetUserIDFromName( EditTestOwnerList.SelectedItem.Text );
                test.ExecutionPath = EditTestExecutionPath.Text;
                test.ExecutionBinary = EditTestExecutionName.Text;
                test.ExecutionArgs = EditTestExecutionArgs.Text;
                test.OperatingSystem = EditTestOS.SelectedValue;
                test.OSVersion = EditTestOSVersion.SelectedItem.Text;
                test.RunFrequency = new RunFrequency( DateTime.Now,
                    new TimeSpan( System.Convert.ToInt32( RunFreqMonths.Text )*30 + 
                        System.Convert.ToInt32( RunFreqDays.Text ),
                        System.Convert.ToInt32( RunFreqHours.Text ),
                        System.Convert.ToInt32( RunFreqMinutes.Text ),
                        0,
                        0 ),
                    System.Convert.ToInt32( EditTestRunNumberOfTimes.Text ),
                    EditTestInfinite.Checked ).GetFrequencyString( );
                
                // Get the are to which the test should belong...
                test.AreaID = AreaHolder.Text;

                dbConnection.AddTest( test );

                // Refresh the listing of information...
                ShowSelectedAreaInformation( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ) );

                // Repopulate the tree view...
                PopulateTreeview( );

                this.ExpandTreeNodes( );
                this.SetSelectedNode( );
            }
            else
            {
                // Set the currently edited test ID...
                test = dbConnection.RetrieveTest( EditTestInternalID.Text );

                // here we are updating an existing test...
                if ( EditTestTestPriorityListBox.SelectedItem == null )
                {
                    PriorityRequiredError.Text = "You must enter a priority.";

                    return;
                }

                test.Name = EditTestTestName.Text;
                test.Description = EditTestTestDescription.Text;
                test.Priority = EditTestTestPriorityListBox.SelectedItem.Text.Replace( "P", "" );
                test.OwnerID = dbConnection.GetUserIDFromName( EditTestOwnerList.SelectedItem.Text );
                test.ExecutionPath = EditTestExecutionPath.Text;
                test.ExecutionBinary = EditTestExecutionName.Text;
                test.ExecutionArgs = EditTestExecutionArgs.Text;
                test.OperatingSystem = EditTestOS.SelectedValue;
                test.OSVersion = EditTestOSVersion.SelectedItem.Text;
                test.RunFrequency = new RunFrequency( DateTime.Now,
                    new TimeSpan( System.Convert.ToInt32( RunFreqMonths.Text )*30 + 
                    System.Convert.ToInt32( RunFreqDays.Text ),
                    System.Convert.ToInt32( RunFreqHours.Text ),
                    System.Convert.ToInt32( RunFreqMinutes.Text ),
                    0,
                    0 ),
                    System.Convert.ToInt32( EditTestRunNumberOfTimes.Text ),
                    EditTestInfinite.Checked ).GetFrequencyString( );                                

                // Update the information for the test...
                dbConnection.UpdateTest( test.ID, test );

                // Refresh the listing of information...
                if ( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ).Type == "Area" )
                    ShowSelectedAreaInformation( TestTreeView.GetNodeFromIndex( TestTreeView.SelectedNodeIndex ) );

                // Repopulate the tree view...
                PopulateTreeview( );

                this.ExpandTreeNodes( );
                this.SetSelectedNode( );
            }
        }

        /// <summary>
        /// Displays the history of the specified test ID...
        /// </summary>
        /// <param name="testID"></param>
        private void ShowHistoryInformation( string testID )
        {
            HistoryPanel.Visible = true;
            AreaInfoPanel.Visible = false;
            TestEditPanel.Visible = false;

            DataTable historyTable = new DataTable( );

            historyTable.Columns.Add( "Run Time", typeof( string ) );
            historyTable.Columns.Add( "Status", typeof( string ) );

            foreach ( TestHistory testHistory in dbConnection.RetrieveTestHistories( testID ) )
            {
                historyTable.Rows.Add( new string [] {
                                                         testHistory.Runtime.ToString( ),
                                                         testHistory.Status } );                                                           
            }

            // Assign the data source and bind it...
            TestHistoryGrid.DataSource = historyTable;
            TestHistoryGrid.DataBind( );
        }

        private void AreaTestsGrid_ItemCommand(object source, System.Web.UI.WebControls.DataGridCommandEventArgs e)
        {
            // Make sure this is a history command...
            if ( e.CommandName == "History" )
                ShowHistoryInformation( e.Item.Attributes["ID"] );
        }

        private void AreaTestsGrid_PageIndexChanged(object source, System.Web.UI.WebControls.DataGridPageChangedEventArgs e)
        {
            AreaTestsGrid.CurrentPageIndex = e.NewPageIndex;
        }       
    }
}
