function fire_site_license_changed()
{
	if(document.generate_license.site_license.checked == true)
	{

		document.generate_license.licenses_purchased.style.display = 'none';
		document.generate_license.licenses_purchased.disabled = true;
		document.generate_license.licenses_purchased.value = '';
		document.getElementById('license_count_label').innerHTML = '';
	}
	else
	{
		document.generate_license.licenses_purchased.style.display = '';
    	document.generate_license.licenses_purchased.disabled = false;
		document.getElementById('license_count_label').innerHTML = '# Licenses Purchased:';
	}
	//document.generate_license.licenses_purchased
}


function fire_expiration_changed()
{
	//
    if(document.generate_license.expiration.checked == true)
    {
    	document.generate_license.valid_license_days.style.display = 'none';
	document.generate_license.valid_license_days.disabled = true;
	document.generate_license.valid_license_days.value = '';
	document.getElementById('license_days_label').innerHTML = '';
    }
    else
    {
	document.generate_license.valid_license_days.style.display = '';
    	document.generate_license.valid_license_days.disabled = false;
    	document.getElementById('license_days_label').innerHTML = '# Days Valid:';
    }
	 
}

function fire_customer_email_changed()
{
	if(document.generate_license.customer_email.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_email(document.generate_license.customer_email))
	{
		document.generate_license.customer_email.value = '';
		return overlib('Email addresses take the form of:<br/>username@domain.com', CAPTION, 'Invalid Email Address', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}

function fire_customer_name_changed()
{
	if(document.generate_license.customer_name.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_name(document.generate_license.customer_name))
	{
		document.generate_license.customer_name.value = '';
		return overlib('Customer names may contain letters, spaces, periods, dashes, and hyphens', CAPTION, 'Invalid Customer Name', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}

function fire_company_name_changed()
{
	if(document.generate_license.company_name.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_companyname(document.generate_license.company_name))
	{
		document.generate_license.company_name.value = '';
		return overlib('Company names may contain letters, numbers, spaces, periods, commas, dashes, and hyphens', CAPTION, 'Invalid Company Name', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}




function fire_lp_changed()
{
	if(document.generate_license.licenses_purchased.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_quantity(document.generate_license.licenses_purchased))
	{
		document.generate_license.licenses_purchased.value = '';
		return overlib('The number of licenses purchased must be number greater then 0, such as 2 or 459', CAPTION, 'Invalid Quantity', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}

function fire_ld_changed()
{
	if(document.generate_license.valid_license_days.value.length == 0)
	{
	  return;	
	}
	
	if(!is_valid_quantity(document.generate_license.valid_license_days))
	{
		document.generate_license.valid_license_days.value = '';
		return overlib('The number of valid license days must be number greater then 0, such as 2 or 459', CAPTION, 'Invalid Quantity', FADEOUT, SHADOW,  CENTERPOPUP)		
	}
}