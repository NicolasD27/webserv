<?php
header_remove();
header('Content-Type: text/html');
header_remove('Content-Type');
?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title>Info php</title>
	</head>
	<body>	
		<?php
			 phpinfo();
		?>
	</body>
</html>