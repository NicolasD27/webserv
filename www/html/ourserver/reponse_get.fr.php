<?php
	header_remove();
    header('Content-Type: text/html');
    header_remove('Content-Type');
?>
<!DOCTYPE html>
<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<title>Réponse GET</title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
		<div id="main">
    	<content>
			<p>Réponse du Formulaire GET ! </p>
			<?php
				echo "<p>".$_GET["say"]." à ".$_GET["to"]."</p>";
			?>
			<hr>
			<h3><a href="/">retour vers répertoire racine.</a></h3>
		</content>
		</div>
	</body>
</html>