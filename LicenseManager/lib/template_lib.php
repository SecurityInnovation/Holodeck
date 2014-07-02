<?php
  /**
   * html generating template functions and fields
   */

  require_once('Smarty.class.php');

  $smarty = new Smarty;

  //references the template directory
  $smarty->template_dir = './smarty/templates/';
  $smarty->compile_dir = './smarty/templates_c/';
  //$smarty->config_dir = './smarty/configs/';

  $smarty->compile_check = TRUE;
  //$smarty->debugging = TEMPLATE_DEBUGGING;
?>