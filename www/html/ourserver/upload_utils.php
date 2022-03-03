<?php
header_remove();
header('Content-Type: text/html');
header_remove('Content-Type');

function return_bytes($val) {
  $val = trim($val);
  $ret = (int)$val;
  $last = strtolower($val{strlen($val)-1});
  switch($last) {
      case 'g':
          $ret *= 1024;
      case 'm':
          $ret *= 1024;
      case 'k':
          $ret *= 1024;
  }
  return $ret;
}
?>