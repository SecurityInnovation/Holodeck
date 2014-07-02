<%@ Page language="c#" Codebehind="testtracker.aspx.cs" AutoEventWireup="true" Inherits="testtracker.TTWebForm" %>
<%@ Register TagPrefix="iewc" Namespace="Microsoft.Web.UI.WebControls" Assembly="Microsoft.Web.UI.WebControls" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
    <HEAD>
        <title>Test Tracker</title>
        <meta name="vs_snapToGrid" content="True">
        <meta name="vs_showGrid" content="True">
        <meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
        <meta content="C#" name="CODE_LANGUAGE">
        <meta content="JavaScript" name="vs_defaultClientScript">
        <meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
    </HEAD>
    <body MS_POSITIONING="GridLayout">
        <form id="Form1" method="post" runat="server">
            <asp:panel id="TitlePanel" style="Z-INDEX: 101; LEFT: 8px; POSITION: absolute; TOP: 8px" runat="server"
                Width="136px" Height="24px" BackColor="#E0E0E0">
                <STRONG>&nbsp;Test Tracker v 1.0</STRONG></asp:panel>
            <asp:panel id="HistoryPanel" style="Z-INDEX: 107; LEFT: 184px; POSITION: absolute; TOP: 40px"
                runat="server" BackColor="White" Height="560px" Width="592px" Visible="False" BorderWidth="1px"
                BorderStyle="Solid">
                <P>&nbsp;<BR>
                    &nbsp;<STRONG>Test History</STRONG></P>
                <P align="center">
                    <asp:DataGrid id="TestHistoryGrid" runat="server" Height="1px" Width="575px" CellPadding="0" PageSize="20"
                        AllowSorting="True">
                        <EditItemStyle Height="1px" Width="1px"></EditItemStyle>
                        <AlternatingItemStyle BackColor="#C0FFFF"></AlternatingItemStyle>
                        <ItemStyle Height="1px" Width="1px"></ItemStyle>
                        <HeaderStyle Font-Bold="True" Height="1px" ForeColor="White" Width="1px" BackColor="Black"></HeaderStyle>
                    </asp:DataGrid></P>
            </asp:panel>
            <asp:panel id="TestEditPanel" style="Z-INDEX: 106; LEFT: 184px; POSITION: absolute; TOP: 40px"
                runat="server" Width="592px" Height="560px" BackColor="White" BorderStyle="Solid" BorderWidth="1px"
                Visible="False">
                <P><STRONG><BR>
                        &nbsp;
                        <asp:Label id="TestEditLabel" runat="server">Edit/View Test</asp:Label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        <asp:Label id="AreaHolder" runat="server" Visible="False"></asp:Label><BR>
                        <TABLE id="Table1" style="WIDTH: 584px; BORDER-TOP-STYLE: none; BORDER-RIGHT-STYLE: none; BORDER-LEFT-STYLE: none; HEIGHT: 75px; BORDER-BOTTOM-STYLE: none"
                            cellSpacing="1" cellPadding="1" width="584" border="1">
                            <TR>
                                <TD style="WIDTH: 139px">Name:</TD>
                                <TD>
                                    <asp:TextBox id="EditTestTestName" runat="server" Width="280px"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Description:</TD>
                                <TD style="BORDER-RIGHT: thin; BORDER-TOP: thin; BORDER-LEFT: thin; BORDER-BOTTOM: thin">
                                    <asp:TextBox id="EditTestTestDescription" runat="server" Height="56px" Width="280px" TextMode="MultiLine"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Priority:</TD>
                                <TD>
                                    <asp:ListBox id="EditTestTestPriorityListBox" runat="server" Height="56px" Width="56px">
                                        <asp:ListItem Value="P0">P0</asp:ListItem>
                                        <asp:ListItem Value="P1">P1</asp:ListItem>
                                        <asp:ListItem Value="P2">P2</asp:ListItem>
                                        <asp:ListItem Value="P3">P3</asp:ListItem>
                                        <asp:ListItem Value="P4">P4</asp:ListItem>
                                    </asp:ListBox>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                    <asp:Label id="PriorityRequiredError" runat="server" ForeColor="Red" Font-Bold="True"></asp:Label></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px; HEIGHT: 16px">Owner</TD>
                                <TD style="HEIGHT: 16px">
                                    <asp:DropDownList id="EditTestOwnerList" runat="server" Width="280px"></asp:DropDownList></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Execution Path:</TD>
                                <TD>
                                    <asp:TextBox id="EditTestExecutionPath" runat="server" Width="280px"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Execution Binary/Script:</TD>
                                <TD>
                                    <asp:TextBox id="EditTestExecutionName" runat="server" Width="280px"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Execution Arguments:</TD>
                                <TD>
                                    <asp:TextBox id="EditTestExecutionArgs" runat="server" Width="280px"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Operating System:</TD>
                                <TD>
                                    <asp:DropDownList id="EditTestOS" runat="server" Width="280px">
                                        <asp:ListItem Value="Windows2003">Windows 2003</asp:ListItem>
                                        <asp:ListItem Value="WindowsXP" Selected="True">Windows XP</asp:ListItem>
                                        <asp:ListItem Value="Windows2000">Windows 2000</asp:ListItem>
                                        <asp:ListItem Value="WindowsNT">Windows NT</asp:ListItem>
                                        <asp:ListItem Value="WindowsMe">Windows Me</asp:ListItem>
                                        <asp:ListItem Value="Windows98">Windows 98</asp:ListItem>
                                        <asp:ListItem Value="Windows95">Windows 95</asp:ListItem>
                                    </asp:DropDownList></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">OS Version:</TD>
                                <TD>
                                    <asp:DropDownList id="EditTestOSVersion" runat="server" Width="280px">
                                        <asp:ListItem Value="Default" Selected="True">Default</asp:ListItem>
                                        <asp:ListItem Value="SP1">SP1</asp:ListItem>
                                        <asp:ListItem Value="SP2">SP2</asp:ListItem>
                                        <asp:ListItem Value="SP3">SP3</asp:ListItem>
                                        <asp:ListItem Value="4.0">4.0</asp:ListItem>
                                        <asp:ListItem Value="4.0 SP1">4.0 SP1</asp:ListItem>
                                        <asp:ListItem Value="4.0 SP2">4.0 SP2</asp:ListItem>
                                        <asp:ListItem Value="4.0 SP3">4.0 SP3</asp:ListItem>
                                        <asp:ListItem Value="4.0 SP4">4.0 SP4</asp:ListItem>
                                        <asp:ListItem Value="4.0 SP5">4.0 SP5</asp:ListItem>
                                        <asp:ListItem Value="4.0 SP6">4.0 SP6</asp:ListItem>
                                        <asp:ListItem Value="3.51">3.51</asp:ListItem>
                                    </asp:DropDownList></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Run Frequency:</TD>
                                <TD>
                                    <P>Run Every
                                        <asp:TextBox id="RunFreqMonths" runat="server" Width="24px"></asp:TextBox>Months,
                                        <asp:TextBox id="RunFreqDays" runat="server" Width="24px"></asp:TextBox>&nbsp;Days,
                                        <asp:TextBox id="RunFreqHours" runat="server" Width="24px"></asp:TextBox>&nbsp;Hours,
                                        <asp:TextBox id="RunFreqMinutes" runat="server" Width="24px"></asp:TextBox>&nbsp;Mins 
                                        for&nbsp;</P>
                                    <P>
                                        <asp:RadioButton id="EditTestInfiniteNoCare" runat="server" GroupName="EditInfinite" Checked="True"></asp:RadioButton>
                                        <asp:TextBox id="EditTestRunNumberOfTimes" runat="server" Width="32px"></asp:TextBox>&nbsp;times&nbsp; 
                                        &nbsp;<STRONG>or</STRONG>&nbsp;&nbsp;
                                        <asp:RadioButton id="EditTestInfinite" runat="server" Text="Infinite" GroupName="EditInfinite"></asp:RadioButton></P>
                                </TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">State:
                                </TD>
                                <TD>
                                    <asp:TextBox id="EditTestState" runat="server" Width="280px" ReadOnly="True"></asp:TextBox></TD>
                            </TR>
                            <TR>
                                <TD style="WIDTH: 139px">Internal ID:</TD>
                                <TD>
                                    <asp:Label id="EditTestInternalID" runat="server" Font-Bold="True"></asp:Label></TD>
                            </TR>
                        </TABLE>
                    </STRONG>
                </P>
                <P><STRONG>&nbsp;&nbsp;
                        <asp:Button id="Button1" runat="server" Text="Save Test"></asp:Button></STRONG></P>
            </asp:panel>
            <asp:panel id="UserPanel" style="Z-INDEX: 102; LEFT: 520px; POSITION: absolute; TOP: 8px" runat="server"
                Width="248px" Height="24px" BackColor="#E0E0E0">
                <STRONG>&nbsp;User:
                    <asp:Label id="UserNameLabel" runat="server" Width="168px" ForeColor="#0000C0" Font-Bold="True"></asp:Label></STRONG></asp:panel>&nbsp;
            <iewc:treeview id="TestTreeView" style="Z-INDEX: 103; LEFT: 8px; POSITION: absolute; TOP: 40px"
                runat="server" Width="168px" Height="560px" BackColor="Transparent" SystemImagesPath="/webctrl_client/1_0/treeimages/"
                BorderColor="Black" BorderStyle="Inset" BorderWidth="1px" AutoPostBack="True" AutoSelect="True">
                <iewc:TreeNode Text="Node0"></iewc:TreeNode>
            </iewc:treeview><asp:panel id="AreaInfoPanel" style="Z-INDEX: 105; LEFT: 184px; POSITION: absolute; TOP: 40px"
                runat="server" Width="592px" Height="560px" BorderStyle="Solid" BorderWidth="1px" BackColor="White" Visible="False">
<P><BR>
                    &nbsp;
                    <asp:Button id="AddNewTestButton" runat="server" Width="224px" Text="Add New Test to Area"></asp:Button>&nbsp;
                    <asp:Button id="DeleteArea" runat="server" Width="104px" Text="Delete Area"></asp:Button>&nbsp;&nbsp;
                    <asp:Button id="AddArea" runat="server" Width="112px" Text="Add Child Area"></asp:Button>
                    <asp:TextBox id="AreaNameToAdd" runat="server" Width="80px">Child Name</asp:TextBox><BR>
                    <asp:Label id="AreaSelectedTitle" runat="server" Font-Bold="True">Area Selected: </asp:Label>&nbsp;
                    <asp:Label id="AreaSelectedValue" runat="server"></asp:Label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<BR>
                    <STRONG>Number of Tests in Area:
                        <asp:Label id="NumberOfTestsInAreaValue" runat="server"></asp:Label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        <asp:Label id="ErrorMessage" runat="server" ForeColor="Red" Font-Bold="True"></asp:Label></STRONG><STRONG></P>
<P align="center">
                    <asp:DataGrid id="AreaTestsGrid" runat="server" Height="1px" Width="568px" CellPadding="0" PageSize="15"
                        AllowSorting="True">
                        <AlternatingItemStyle BackColor="#C0FFFF"></AlternatingItemStyle>
                        <ItemStyle Height="1px"></ItemStyle>
                        <HeaderStyle Font-Bold="True" Height="1px" ForeColor="White" BackColor="Black"></HeaderStyle>
                        <Columns>
                            <asp:EditCommandColumn ButtonType="PushButton" UpdateText="Update" CancelText="Cancel" EditText="Edit">
                                <HeaderStyle Width="5px"></HeaderStyle>
                            </asp:EditCommandColumn>
                            <asp:ButtonColumn Text="History" ButtonType="PushButton" CommandName="History"></asp:ButtonColumn>
                            <asp:ButtonColumn Text="Delete" ButtonType="PushButton" CommandName="Delete"></asp:ButtonColumn>
                        </Columns>
                    </asp:DataGrid></P></STRONG></asp:panel>
            <asp:Label id="SelectAnAreaOrTest" style="Z-INDEX: 100; LEFT: 352px; POSITION: absolute; TOP: 256px"
                runat="server" Width="217px" BorderWidth="1px" BorderStyle="Solid" Font-Bold="True" BorderColor="Navy">Please Select an Area or Test</asp:Label></form>
    </body>
</HTML>
