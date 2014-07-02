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

    <script type="text/javascript" src="includes/scripts/overlib.js"><!-- overLIB (c) Erik Bosrup --></script>
    <script type="text/javascript" src="includes/scripts/overlib_centerpopup.js"><!-- overLIB (c) Erik Bosrup --></script>
    <script type="text/javascript" SRC="includes/scripts/overlib_shadow.js"></script>
    <script type="text/javascript" SRC="includes/scripts/overlib_fade.js"></script>  

  <script type="text/javascript">
  <!--
  overlib_pagedefaults(FGCOLOR, '#ffffff', BGCOLOR, '#D2D8EA', TEXTCOLOR, '#666666');
  //-->
  </script>
  </head>
  <body>
  
 <div id="top">
     <img src="includes/img/interface/top-logo.png" alt="SI Logo"/>
  </div>
	
	<div id="main">
	<div id="fullwidth">
	<h2>Licenses</h2>
	{if $show_top_url}
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
	  <br/>
	  <br/>
	  <br/>
	  <br/>
	  <br/>
	 {/if}
	<h4>entries</h4>
    <br/>
   <table class="license_list" cellspacing="0" cellpadding="2">
    <tr class="textname">
     <!-- <td>Customer Name</td> -->
      <td>Company Name</td>
      <td>length</td>
      <td>licenses</td>
      <td>product</td>
      <td>created</td>
    </tr>
   
    {section name=customer loop=$licenses}
    <tr class="{$licenses[customer].Class}">
      <td><a href="javascript:void(0);" onmouseout="return nd()" onmouseover="return overlib('Customer Name: <b>{$licenses[customer].CustomerName}</b><br/> Serial Number: <b>{$licenses[customer].SerialNumber}</b><br/> Registration Key: <b>{$licenses[customer].RegistrationKey}</b><br/>', CAPTION, 'License Summary')">{$licenses[customer].CompanyName}</a> </td>
      <td>{$licenses[customer].DaysValid}</td>
      <td>{$licenses[customer].LicensesPurchased}</td>
      <td>{$licenses[customer].Product}</td>
      <td>{$licenses[customer].timeof_creation}</td>
    </tr>
    {/section}

     </table>
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

	<div id="footer"></div>
  </div>
  <div id="bottom"></div> 
</body>
</html>