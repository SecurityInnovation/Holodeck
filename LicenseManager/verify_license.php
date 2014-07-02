<?php

require_once('lib/template_lib.php');
require_once('lib/license_lib.php');
require_once('lib/security_lib.php');
require_once('lib/db_lib.php');

$serial_number = FALSE;
$registration_key = FALSE;

$db_conn = create_db_connection();


$serial_number = null;
$registration_key = null;

//verify the serial number and registration key don't contain any dangerous characters
if(isset($_POST['serial_number'])){
	if(validate_alphanumeric_pattern($_POST['serial_number']))
	{
		$serial_number = $_POST['serial_number'];
	}
}
if(isset($_POST['registration_key'])){
	if(validate_registration_key($_POST['registration_key']))
	{
		$registration_key = $_POST['registration_key'];
	}
}

$result = false;

if($serial_number != null && $registration_key != null){

	//test registration key decoding
	$result = DecodeRegistrationKey($serial_number, $registration_key);

	if($result !== false)
	{
		$smarty->assign('serial_number', $result->serialNumber);
		$smarty->assign('registration_key', $result->registrationKey);
		if($result->numberOfLicensesPurchased == 0)
		{
			$smarty->assign('license_count', 'site_license');
		}
		else
		{
			$smarty->assign('license_count', $result->numberOfLicensesPurchased);
		}
		if($result->numberOfDaysValid == 0)
		{
			$smarty->assign('days_valid', 'non-expiring');
		}
		else
		{
			$smarty->assign('days_valid', $result->numberOfDaysValid);
		}
		$smarty->assign('valid_license', true);
		$smarty->assign('key_type', get_key_type_name($result->keyVersion));
		$smarty->assign('customer_type', get_customer_type_name($result->customerType));
		$smarty->assign('product_type', get_product_type_name($result->productType));
		$smarty->assign('functionality_type', get_functionality_type_name($result->functionalityType));
	}
	else
	{
        $smarty->assign('valid_license', false);
	}
	
	$smarty->assign('submitted', true);
}



$smarty->display('verify_license.tpl');
mysql_close($db_conn);     //close the database connection
?>