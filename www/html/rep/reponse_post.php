<?php
	function proper_parse_str($str) {
	  # result array
	  $arr = array();
	
	  # split on outer delimiter
	  $pairs = explode('&', $str);
	
	  # loop through each pair
	  foreach ($pairs as $i) {
		# split into name and value
		list($name,$value) = explode('=', $i, 2);
	   
		# if name already exists
		if( isset($arr[$name]) ) {
		  # stick multiple values into an array
		  if( is_array($arr[$name]) ) {
			$arr[$name][] = $value;
		  }
		  else {
			$arr[$name] = array($arr[$name], $value);
		  }
		}
		# otherwise, simply stick it in a scalar
		else {
		  $arr[$name] = $value;
		}
	  }
	
	  # return result array
	  return $arr;
	}
	
?>
<!DOCTYPE html>
<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<title>Réponse POST</title>
	</head>
	<body>
		<p>Réponse du Formulaire POST ! </p>	
		<?php
			//echo "ici".$_POST["say"];
			phpinfo();
		?>
	</body>
</html>