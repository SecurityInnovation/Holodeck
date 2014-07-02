<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
            "http://www.w3.org/TR/html4/strict.dtd">
<html lang="en">
<head>
  <title>Holodeck License Management</title>
  <!-- <link rel="up" href="contents.html"> -->
  <!-- Include all necessary css files -->
  <link type="text/css" rel="stylesheet" href="includes/css/largesheet.css">

  <link rel="icon" href="includes/img/interface/favicon.ico" type="image/x-icon">
  <link rel="shortcut icon" href="includes/img/interface/favicon.ico" type="image/x-icon">

  <script type="text/javascript" src="includes/scripts/overlib.js"></script>
  <script type="text/javascript" src="includes/scripts/overlib_fade.js"></script>
  <script type="text/javascript" src="includes/scripts/overlib_shadow.js"></script>
  <script type="text/javascript" src="includes/scripts/overlib_centerpopup.js"></script>
  <script type="text/javascript" src="includes/scripts/validation.js"></script>
  <script type="text/javascript" src="includes/scripts/verify_license_ui.js"></script>

{literal}
<script type="text/javascript">
<!--

function submit_form()
{
  document.verify_license.submit()
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
	  <h2>License Verification</h2>
	</div>

	<div class="one_half">
  	  <h4>Verify License</h4>
      <form name="verify_license" method="post" action="verify_license.php">
        <table class="editor" border="0" cellspacing="0">
  	  	  <tr class="textname">
		    <td class="label">Serial Number: </td>
		    <td><input type="text" name="serial_number" value="" onkeypress="nd()" onchange="fire_serial_number_changed()" /></td>
		  </tr>
		  <tr class="textname">
		    <td class="label">Registration Key:</td>
		    <td><input type="text" name="registration_key" value="" onkeypress="nd()" onchange="fire_registration_key_changed()" /></td>
		  </tr>	
		  <tr class="textname">
		    <td colspan="2" class="label">
                          <input type="button" name="verify_license" value="verify" onclick="submit_form()"/>
	        </td>
	      </tr>		
	    </table>
      </form>
	
	  <br/>
	  <div class="sideitems">
        <table>
          <tr>
            <td class="image_col">
              <img src="includes/img/interface/home.png" alt="Generate License Home Logo"/>
            </td>
            <td>
              <ul>
                <li>
 	              <a href="index.php">
		            <em>Return</em>
		           <span>Go back to main menu</span>
		          </a>
	            </li>
              </ul>
            </td>
          </tr>
        </table> 
	  </div>
	</div>
	<div class="one_half">
	  <h4>License Details</h4>
	  {if $valid_license}
		
	<h3>Success!</h3><small>decoded the registration key and serial number:</small><br/>
	 serial number: {$serial_number}<br/>
     registration key: {$registration_key}<br/>
     number of licenses purchased: {$license_count}<br/>
     number of days valid: {$days_valid}<br/>
     key type: {$key_type}<br/>
     customer type: {$customer_type}<br/>
     product type: {$product_type}<br/>
     functionality type: {$functionality_type}<br/>
	
     {elseif $submitted}
		The serial number and registration key pair is not valid<br/>
	 {/if}
    </div>
	<div id="footer"></div>
  </div>

  <div id="bottom"></div>
    
</body>
</html>