<?php

function create_db_connection()
{
	$db_conn = mysql_connect('localhost', 'license_manager', 'sb32pg')
	or die('Could not connect: ' . mysql_error());
	mysql_select_db('license_manager', $db_conn) or die('Could not select database');
	
	return $db_conn;
}
?>