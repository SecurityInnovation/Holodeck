<?php 
require_once('lib/license_lib.php');
require_once('lib/template_lib.php');
require_once('lib/db_lib.php');

$db_conn = create_db_connection();

$query = "SELECT ID, CompanyName, CustomerName, SerialNumber, RegistrationKey, DaysValid, LicensesPurchased, Product, ".
//"CustomerName, CustomerType, KeyVersion, FunctionalityLevel, ".
"timeof_creation FROM RegData ORDER BY timeof_creation DESC";
$result = mysql_query($query);

//check mysql result set
if($result)
{
	$licenses = array();
	while ($row = mysql_fetch_assoc($result))
	{
		$row['Class'] = "tablecol" . ((count($licenses) % 2) + 1);
		if($row['DaysValid'] == 0)
		{
			$row['DaysValid'] = 'non-expiring';
		}
		else
		{
			$row['DaysValid'] .= ' days';
		}
		if($row['LicensesPurchased'] == 0)
		{
			$row['LicensesPurchased'] = "unlimited <i>(site)</i>";
		}
		$row['Product'] = get_product_type_name($row['Product']);

		if($row['timeof_creation'] == 0)
		{
			$row['timeof_creation'] = "<i>n/a</i>";
		}
		else
		{
			$row['timeof_creation'] = strftime("%B %d, %Y", $row['timeof_creation']);
		}
		$licenses[] = $row;
	}
	$smarty->assign('licenses', $licenses);
	$smarty->assign('show_top_url', count($licenses) > 30);
	mysql_free_result($result);    //release mysql result set
}

mysql_close($db_conn);    //close the database connection
$smarty->display('view_licenses.tpl');

/*
<!--	
   else
   {
   	echo "<tr><td colspan=\"11\">No licenses are in the database</td></tr>";
   }
   -->
*/
?>