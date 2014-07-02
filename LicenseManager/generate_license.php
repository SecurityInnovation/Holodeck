<?php 
require_once('lib/license_lib.php');
require_once('lib/template_lib.php');
require_once('lib/db_lib.php');

$customer_name = null;
$company_name = null;
$customer_email = null;
$product_type = null;
$key_version = null;
$valid_license_days = null;
$licenses_purchased = null;
$functionality_type = null;
$customer_type = null;


//run this from the command line to setup an account (will prompt you for root pwd)
//mysql dbname -u root -p  -e "GRANT SELECT, INSERT, UPDATE, DELETE ON dbname.* TO username@localhost IDENTIFIED BY 'passwords'"

$db_conn = create_db_connection();


$mode = 'display_form';


function get_serial_number()
{
	$query = "SELECT MAX(SerialNumber) FROM RegData";
	$result = mysql_query($query);
	if($result)
	{
		$row = mysql_fetch_row($result);
		//free the result set
		mysql_free_result($result);
		return $row[0] + 1;
	}
	//the starting serial number is 1000000
	return '1000000';
}


function store_license($license, $customer_name, $company_name, $customer_email)
{
	$timeof_creation = time();
	$query = "INSERT INTO RegData (CustomerName, ".
	"CompanyName, ".
	"CustomerEmail, ".
	"SerialNumber, ".
	"RegistrationKey, ".
	"DaysValid, ".
	"LicensesPurchased, ".
	"Product, ".
	"CustomerType, ".
	"KeyVersion, ".
	"FunctionalityLevel, ".
	"timeof_creation) VALUES ".
	"('$customer_name', '$company_name', '$customer_email', ".
	"'".$license->serialNumber."', '".$license->registrationKey."', ".
	$license->numberOfDaysValid . ", ". $license->numberOfLicensesPurchased .
	", " . $license->productType . ", " . $license->customerType. ", ".
	$license->keyVersion . ", " . $license->functionalityType. ", $timeof_creation)";

	//echo "<!-- query: \n$query\n -->";
	$result = mysql_query($query);
	if($result)
	{
		return TRUE;
	}
	return FALSE;
}



if (isset($_POST['generate_license']) && $_POST['generate_license'] == 'create')
{
	//verify all of the POST fields
	require_once('lib/security_lib.php');

	if(isset($_POST['customer_name']))
	{
		//validate the customer's name
		if(validate_name_pattern($_POST['customer_name']))
		{
			$customer_name = $_POST['customer_name'];
			$smarty->assign('customer_name', $customer_name);
		}
	}

	if(isset($_POST['company_name']))
	{
		//validate the company's name
		if(validate_name_pattern($_POST['company_name']))
		{
			$company_name = $_POST['company_name'];
			$smarty->assign('company_name', $company_name);
		}
	}

	if(isset($_POST['customer_email']))
	{
		//validate the customer's name
		if(validate_email_pattern($_POST['customer_email']))
		{
			$customer_email = $_POST['customer_email'];
			$smarty->assign('customer_email', $customer_email);
		}
	}


	if(isset($_POST['product_type']))
	{
		//validate the product type enumeration
		if(validate_product_type($_POST['product_type']))
		{
			$product_type = $_POST['product_type'];
			$smarty->assign('product_type', $product_type);
		}
	}

	if(isset($_POST['key_version'])){
		//validate the key version enumeration
		if(validate_key_version($_POST['key_version']))
		{
			$key_version = $_POST['key_version'];
			$smarty->assign('key_version', $key_version);
		}
	}

	if(isset($_POST['functionality_type'])){
		//validate the functionality type enumeration
		if(validate_functionality_type($_POST['functionality_type']))
		{
			$functionality_type = $_POST['functionality_type'];
			$smarty->assign('functionality_type', $functionality_type);
		}
	}

	if(isset($_POST['customer_type']))
	{
		//validate the customer type enumeration
		if(validate_customer_type($_POST['customer_type']))
		{
			$customer_type = $_POST['customer_type'];
			$smarty->assign('customer_type', $customer_type);
		}
	}


	if(isset($_POST['valid_license_days']))
	{
		//ensure valid days is an integer between 0 and 999 (inclusive)
		if(validate_integer_pattern($_POST['valid_license_days'], 0, 999))
		{
			$valid_license_days = $_POST['valid_license_days'];
			$smarty->assign('valid_license_days', $valid_license_days);
		}
	}

	if(isset($_POST['licenses_purchased']))
	{
		//ensure valid days is an integer between 0 and 10,000 (inclusive)
		if(validate_integer_pattern($_POST['licenses_purchased'], 0, 10000))
		{
			$licenses_purchased = $_POST['licenses_purchased'];
			$smarty->assign('licenses_purchased', $licenses_purchased);
		}
	}

	if(isset($_POST['site_license']))
	{
		if($_POST['site_license'] == 'on')
		{
			$licenses_purchased = 0;
			$smarty->assign('licenses_purchased', $licenses_purchased);
		}
	}

	if(isset($_POST['expiration']))
	{
		if($_POST['expiration'] == 'on')
		{
			$valid_license_days = 0;
			$smarty->assign('valid_license_days', $valid_license_days);
		}
	}
	
	
	//ensure all data is valid before generating the license
	if($customer_name !== null && $customer_email !== null &&
	$product_type !== null && $key_version !== null &&
	$valid_license_days !== null && $licenses_purchased !== null &&
	$functionality_type !== null && $customer_type !== null)
	{

		//get the next serial number from the database
		$serial_number = get_serial_number();

		//validate that serial number is valid here before proceeding
		//(1000000 is the starting value for the serial numbers)
		if(is_numeric($serial_number) && $serial_number >= 1000000)
		{
			//generate the license
			$rd = new RegistrationData;

			//MR: if number of days valid equals 0, key does not expire
			$rd->numberOfDaysValid = $valid_license_days;
			//MR: if number of licenses purchased equals 0, this is a site license
			$rd->numberOfLicensesPurchased = $licenses_purchased;
			$rd->serialNumber = $serial_number;
			$rd->productType = $product_type;
			$rd->keyVersion = $key_version;
			$rd->functionalityType = $functionality_type;
			$rd->customerType = $customer_type;
			$rd = generate_registration_key($rd);

			//insert the license into the database
			if(store_license($rd, $customer_name, $company_name, $customer_email) == FALSE)
			{
				echo "Warning: license could not be saved to the database!<br/>";
			}

			//display license data
			$smarty->assign('serial_number', $rd->serialNumber);
			$smarty->assign('registration_key', $rd->registrationKey);
			$mode = 'display_add_license_success_page';
		}
	}
	else
	{
		//TODO: print message explaining what data is missing/wrong
	}
}

//close the database connection
mysql_close($db_conn);



//TODO: replace static drop downs in html with dynamic content
//      (pull enumerations from db)
//TODO: make drop downs select values when generate license fails on partial missing data
if($mode == 'display_form')
{
	$smarty->display('generate_license_form.tpl');
}
else if($mode == 'display_add_license_success_page')
{
	$smarty->display('generate_license_success.tpl');
}
?>
  
