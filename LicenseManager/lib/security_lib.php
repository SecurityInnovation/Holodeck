<?php

/**
 * security and validation functions
 *
 * The following functions constitute a simple library for performing input validation and filtering,
 * and add new patterns as you discover them: http://www.regexplib.com/DisplayPatterns.aspx
 * for a really decent tutorial on regular expressions, go here:
 * http://weblogtoolscollection.com/regex/regex.php
 */


define('SALT_LENGTH', 16);


/**
 * Validate a user name
 *
 * Names may include Alphanumeric, spaces, hyphens apostrophes, commas, dashes, and spaces
 * Names must be at least 2 characters long.
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $name check for validity as a name
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */

function validate_name_pattern($name){

	if(strlen($name) < 2)
	  return FALSE;

	return TRUE;

	if (ereg ('^[a-zA-Z0-9\s.\-_\']+$', $name, $match_results)) {
	  return TRUE;
	}
	return FALSE;
}


/**
 * Validate a string for alphanumeric characters (numbers and english letters only)
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $input check for validity as an alphanumeric string
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */
function validate_alphanumeric_pattern($input){
	if(!$input)
	return FALSE;

	$match_results = array();
	if (ereg ('^[a-zA-Z0-9]+$', $input, $match_results)) {
		return TRUE;
	}

	return FALSE;
}


/**
 * Validate a string for alphanumeric characters and dashes(english letters only)
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $input check for validity as a registration key
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */
function validate_registration_key($input){
	if(!$input)
	return FALSE;

	$match_results = array();
	if (ereg ('^[a-zA-Z0-9\-]+$', $input, $match_results)) {
		return TRUE;
	}

	return FALSE;
}



/**
 * Validate an email address
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $email the string to check for validity as an email address
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */
function validate_email_pattern($email){

	$match_results = FALSE;
	if(eregi('^([_a-z0-9-]+)(\.[_a-z0-9-]+){0,2}@[a-z0-9-]+(\.[a-z0-9-]+){0,2}(\.[a-z]{2,3})$', $email, $match_results)) {
		return TRUE;
	}

	return FALSE;
}


/**
 * Validate an email domain
 *
 * An email domain is the part of the email address after the '@' symbol
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $email_domain the string to check for validity as an email domain
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */
function validate_email_domain_pattern($email_domain){

	$match_results = FALSE;

	if(eregi('^[a-z0-9-]+(\.[a-z0-9-]+){0,2}(\.[a-z]{2,3})$', $email_domain, $match_results)) {
		return TRUE;
	}

	return FALSE;
}


/**
 * Validate a date
 *
 * Dates should be formatted as mm-dd-yyyy or m-d-yyyy or m-d-yy or mm-dd-yy
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $date the string to check for validity as a date
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */
function validate_date_pattern($date){

	$match_results = FALSE;

	if(ereg('^([0-9]{1,2})-([0-9]{1,2})-([0-9]{2,4})$', $date, $match_results)) {
		$arr = explode('-', $date);
		if(checkdate ( $arr[0], $arr[1], $arr[2]))
		return TRUE;
	}

	return FALSE;
}


/**
 * Validate a non negative integer value
 *
 * {0,1, 2, 3, ...} if bounds are specified, will check the number
 * against those bounds (inclusive)
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $pwd the string to check for validity
 * @param int $start optional start bounds 
 * @param int $end optional end bounds
 * @return boolean TRUE if the pattern is valid, FALSE otherwise
 */
function validate_integer_pattern($number, $start = null, $end = null){

	if(is_numeric($number))
	{
		if((int)$number < 0)
		{
			return FALSE;
		}

		else if((int)$number != $number)
		{
			return FALSE;
		}

		else if($start != null && $number < $start)
		{
			return FALSE;
		}
		else if($end != null && $number > $end)
		{

			return FALSE;
		}
		else{
			return TRUE;
		}
	}

	return FALSE;
}



function validate_product_type($product_type){
	//make sure this is an integer before we try to pass it to the database
	if(validate_integer_pattern($product_type))
	{
		//look up the enumeration in the db
		$query = "SELECT COUNT(*) FROM Products WHERE ID = $product_type";
		$result = mysql_query($query);
		if($result)
		{
			$row = mysql_fetch_row($result);
			if($row[0] > 0)
			{
				mysql_free_result($result);
				return TRUE;
			}
		}
	}

	mysql_free_result($result);
	return FALSE;
}

function validate_key_version($key_version){
	//make sure this is an integer before we try to pass it to the database
	if(validate_integer_pattern($key_version))
	{
		//look up the enumeration in the db
		$query = "SELECT COUNT(*) FROM KeyVersions WHERE ID = $key_version";
		$result = mysql_query($query);
		if($result)
		{
			$row = mysql_fetch_row($result);
			if($row[0] > 0)
			{
				mysql_free_result($result);
				return TRUE;
			}
		}
	}

	mysql_free_result($result);
	return FALSE;
}

function validate_functionality_type($functionality_type){
	//make sure this is an integer before we try to pass it to the database
	if(validate_integer_pattern($functionality_type))
	{
		//look up the enumeration in the db
		$query = "SELECT COUNT(*) FROM Functionality WHERE ID = $functionality_type";
		$result = mysql_query($query);
		if($result)
		{
			$row = mysql_fetch_row($result);
			if($row[0] > 0)
			{
				mysql_free_result($result);
				return TRUE;
			}
		}
	}

	mysql_free_result($result);
	return FALSE;
}

function validate_customer_type($customer_type)
{
	//make sure this is an integer before we try to pass it to the database
	if(validate_integer_pattern($customer_type))
	{
		//look up the enumeration in the db
		$query = "SELECT COUNT(*) FROM CustomerTypes WHERE ID = $customer_type";
		$result = mysql_query($query);
		if($result)
		{
			$row = mysql_fetch_row($result);
			if($row[0] > 0)
			{
				mysql_free_result($result);
				return TRUE;
			}
		}
	}

	mysql_free_result($result);
	return FALSE;
}


/**
 * checks an email address for validity
 *
 * this function is special in that it checks the email prefix
 * <i>(the part before the '@' symbol)</i> to see if it is a valid
 * address by itself<i>(some cell users might do this)</i>.
 *
 * @author Mike Reinstein <mike@himatomo.jp>
 * @param string $email_prefix the part before the '@' symbol
 * @param string $email_domain the part after the '@' symbol
 * @return array full email address, email prefix and email domain if valid address, returns FALSE otherwise
 */
function is_valid_email($email_prefix, $email_domain){

	require_once('constants.php');
	global $EMAIL_DOMAINS, $EMAIL_REGISTRATION_BLACKLIST,  $EMAIL_REGISTRATION_WHITELIST;

	//determine if the email address passed in is valid
	if(validate_email_pattern($email_prefix)){
		$email = $email_prefix;
	}
	else if(validate_email_pattern("$email_prefix@$email_domain")){
		$email = "$email_prefix@$email_domain";
	}
	else{
		return FALSE;
	}

	//if we got here the email must be valid. split it into prefix and domain
	list($eprefix, $edomain) = explode('@', $email);

	//if we allow any addresses, this one is valid so return it
	if(ALLOW_ANY_EMAIL_ADDRESSES == TRUE){

		//if the domain isn't in the list, set domain to other
		//no, the 3 '=' symbols below is NOT a bug its a comparison operatoor
		//this function can return 0, meaning that a match is encountered
		//at the first index of the array. but 0 evaluates to FALSE. FALSE is
		//returned by this function if no entries are found
		if(array_search($edomain, $EMAIL_DOMAINS) === FALSE){
			return array($email, $email, OTHER);
		}
		else{
			return array($email, $eprefix, $edomain);
		}
	}

	//if running restricted registration (cellular email only) and email is in the accepted domain list
	//or in the whitelistt
	else if((array_search($edomain, $EMAIL_DOMAINS) !== FALSE)  || (array_search($email, $EMAIL_REGISTRATION_WHITELIST) !== FALSE) ){
		//if the email is NOT in the black list return the array
		if(array_search($email, $EMAIL_REGISTRATION_BLACKLIST) === FALSE)
		return array($email, $eprefix, $edomain);
	}

	return FALSE;
}


function generate_salted_sha1_hash($plainText, $salt = null)
{
	if ($salt === null)
	{
		$salt = substr(md5(uniqid(rand(), true)), 0, SALT_LENGTH);
	}
	else
	{
		$salt = substr($salt, 0, SALT_LENGTH);
	}

	return $salt . sha1($salt . $plainText);
}

?>
