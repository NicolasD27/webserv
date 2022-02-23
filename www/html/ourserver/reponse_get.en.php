<!DOCTYPE html>
<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<title>GET response</title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
		<div id="main">
    	<content>
			<p>Response from the GET Form!</p>
			<?php
				echo "<p>".$_GET["say"]." to ".$_GET["to"]."</p>";
			?>
			<hr>
			<h3><a href="/">return to root directory.</a></h3>
		</content>
		</div>
	</body>
</html>