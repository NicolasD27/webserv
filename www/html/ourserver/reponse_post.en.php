<?php
header_remove();
header('Content-Type: text/html');
header_remove('Content-Type');
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>POST Response</title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
	<div id="main">
    	<content>
			<p>Response from the POST Form</p>
			<?php
				echo "<h2><p>".$_POST["say"]." to ".$_POST["to"]."</p></h2>";
			?>
			<hr>
			<h3><a href="/">return to root directory.</a></h3>
		</content>
	</div>
	</body>
</html>