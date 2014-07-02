function fire_serial_number_changed()
{
	if(document.verify_license.serial_number.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_serial_number(document.verify_license.serial_number))
	{
		document.verify_license.serial_number.value = '';
		return overlib('The serial number must be a positive number with at least 7 digits (e.g., 1234567 could be valid.)', CAPTION, 'Invalid Serial Number', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}

function fire_registration_key_changed()
{
	if(document.verify_license.registration_key.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_registration_key(document.verify_license.registration_key))
	{
		document.verify_license.registration_key.value = '';
		return overlib('The registration key must consist of only letters and dashes, and be of the form XXXXX-XXXXX-XXXXX-XXXXX', CAPTION, 'Invalid Registration Key', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}

