<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
            "http://www.w3.org/TR/html4/strict.dtd">
<html lang="en">
<head>
  <title>Holodeck License Management</title>
  <link type="text/css" rel="stylesheet" href="includes/css/largesheet.css">

  <link rel="icon" href="includes/img/interface/favicon.ico" type="image/x-icon">
  <link rel="shortcut icon" href="includes/img/interface/favicon.ico" type="image/x-icon">

  <script type="text/javascript" src="includes/scripts/overlib.js"></script>
  <script type="text/javascript" src="includes/scripts/overlib_fade.js"></script>
  <script type="text/javascript" src="includes/scripts/overlib_shadow.js"></script>
  <script type="text/javascript" src="includes/scripts/overlib_centerpopup.js"></script>
  <script type="text/javascript" src="includes/scripts/validation.js"></script>
  <script type="text/javascript" src="includes/scripts/create_license_ui.js"></script>
{literal}
<script type="text/javascript">
<!--

function submit_form()
{
  document.generate_license.submit()
}

overlib_pagedefaults(FGCOLOR, '#ffffff', BGCOLOR, '#E6322B', TEXTCOLOR, '#666666');
//-->
</script>
{/literal}

</head>

<body>
  <div id="top">
     <img src="includes/img/interface/top-logo.png" alt="SI Logo"/>
  </div>
  <div id="main">
	<div id="fullwidth">
	  <h2>Create a new License</h2>
	</div>

		<form name="generate_license" id="generate_license" method="POST" action="generate_license.php">
	  <div class="one_half">
	    <h4>Customer Info</h4>
	    
		<table class="editor" border="0" cellspacing="0">
		  <tr class="textname">
		    <td class="label">Customer Name:</td>
			<td><input type="text" name="customer_name" id="customer_name" onkeypress="nd()" onchange="fire_customer_name_changed()" class="inputtext" value="{$customer_name}" maxlength="50"/></td>
		  </tr>
		  <tr class="textname"><td class="label">Company Name:</td>
 		    <td><input type="text" name="company_name" id="company_name" onkeypress="nd()" onchange="fire_company_name_changed()" class="inputtext" value="{$company_name}" maxlength="50"/></td>
 		  </tr>
		  <tr class="textname">
		    <td class="label">Customer Email:</td>
			<td><input type="text" name="customer_email" id="customer_email" onkeypress="nd()" onchange="fire_customer_email_changed()" class="inputtext" value="{$customer_email}" maxlength="50"/></td>
		  </tr>						
		</table>
		<br />
		
		<h4>License Details</h4>
		<h3>License Quantity</h3><small>Enter the number of licenses to be puchased. A site license means that the customer can use the product on an unlimited number of machines.</small> 
		<table class="editor" border="0" cellspacing="0">
  		  <tr class="textname">
  		    <td><input type="text" class="textname" name="licenses_purchased" id="licenses_purchased" onkeypress="nd()" onchange="fire_lp_changed()" value="{$licenses_purchased}" size="4"/></td>
	        <td><input type="checkbox" name="site_license" id="site_license" onchange="fire_site_license_changed()"/>Site License?</td>
	      </tr>		
		</table>
		<br/>
				   
        <h3>License Duration</h3><small>Enter the length of time for the license(s) to be valid (in days).</small>
        <table class="editor" border="0" cellspacing="0">
		  <tr class="textname">
		    <td><input type="text" name="valid_license_days" id="valid_license_days" onkeypress="nd()" onchange="fire_ld_changed()" value="{$valid_license_days}" size="4"/></td>
	        <td><input type="checkbox" name="expiration" id="expiration" onchange="fire_expiration_changed()" />No Expiration?</td>
	      </tr>		
		</table>

       <input type="hidden" name="generate_license" value="create"/>
       <br/>
       <table class="editor" border="0" cellspacing="0">
		 <tr class="textname">
		   <td> <input type="button" name="create" value="create" onclick="submit_form()"/></td>
	       <td><input type="button" name="cancel" value="cancel" onclick="document.location='index.php'"/></td>
	     </tr>		
	   </table>	
	 </div>

   <div class="one_half">
	 <h4>License Type</h4>
       <table class="editor" border="0" cellspacing="0">
         <tr class="textname"><td class="label">Product:</td>
  		   <td>
  		     <select name="product_type">
               <option value="0"/>Holodeck</option>
             </select>
           </td>
         </tr>
		 <tr class="textname"><td class="label">Customer Type:</td>
		   <td>
		  	 <select name="customer_type">
               <option value="0"/>Normal Customer</option>
               <option value="1"/>Educational Institution</option>
             </select>
		   </td>
		 </tr>
		 <tr class="textname"><td class="label">Key Version:</td>
		   <td>
		 	 <select name="key_version">
               <option value="0"/>Holodeck Basic</option>
             </select>
		   </td>
		 </tr>
		 <tr class="textname">
		   <td class="label">Functionality:</td>
		   <td>
			 <select name="functionality_type">
               <option value="0"/>Feature Disabled Trial</option>
               <option value="1"/>Time Trial</option>
               <option value="2" selected="selected"/>Full Version</option>
             </select>
	       </td>
	     </tr>
	   </table>
	   
	 </div>
	</form>
	<div id="footer"></div>

</div>
    
  </body>
</html>
  