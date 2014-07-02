

function is_valid_name(name)
{
	if(name.length == 0 )
	{
	  return 0;
	}
	
	var field = '' + name.value;
	var regex = /^[a-z\s.\-\']+$/i;
	return regex.test(field);
}

function is_valid_email(email)
{
	if(email.value.length == 0 )
	{
	  return true;
    }

	var field = '' + email.value;
	var regex = /^([_a-z0-9-]+)(\.[_a-z0-9-]+){0,2}@[a-z0-9-]+(\.[a-z0-9-]+){0,2}(\.[a-z]{2,3})$/i;
	return regex.test(field);
}


function is_valid_companyname(orgname) 
{
    var field = '' + orgname.value;
	var regex = /^([a-z0-9\s.,]+){3,28}$/i;
	return regex.test(field);
}


function is_valid_quantity(num){
    
    var field = '' + num.value;
	var regex = /^([1-9]){1}([0-9]){0,8}$/i;
	return regex.test(field);
}


function is_valid_serial_number(serial)
{
	if(serial.value.length < 7)
	{
		return false;
	}

	var field = serial.value;
	var regex = /^[0-9]+$/i;
	return regex.test(field);
	return false;
}

function is_valid_registration_key(reg)
{
	if(reg.value.length != 23)
	{
		return false;
	}

	var field = reg.value;
	var regex = /^([a-z]){5}\-([a-z]){5}\-([a-z]){5}\-([a-z]){5}$/i;
	return regex.test(field);
	return false;
}


