<?php
header_remove();
header('Content-Type: text/html');
header_remove('Content-Type');
?>

<!DOCTYPE html>
<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<title>Réponse POST</title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
	<div id="main">
    	<content>
			<p>Réponse du Formulaire POST ! </p>
			<?php
				echo "<h2><p>".$_POST["say"]." à ".$_POST["to"]."</p></h2>";
			?>
			<hr>
			<h3><a href="/">retour vers répertoire racine.</a></h3>
		</content>
	</div>
	</body>
</html>