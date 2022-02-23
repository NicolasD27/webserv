<?php
	header_remove();
    header('Content-Type: text/html');
    header_remove('Content-Type');
?>
!DOCTYPE html>
<html lang="fr">
	<head>
		<meta charset="UTF-8">
		<title>42 - Webserv Téléchargements</title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
    <div id="main">
		<h1>Téléchargement ! </h1>
		<?php
$target_dir = $_SERVER['UPLOAD_DIR'];
$target_file = $target_dir."/". basename($_FILES["file"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
//echo $_FILES["file"]["tmp_name"]." -> ".$target_file;

if (!file_exists($target_dir) && !is_dir($target_dir)) 
{
  echo "<p>Désolé, le répertoire [".$target_dir."] n'existe pas.</p>";   
  //echo "<p>Sorry, The directory [".$target_dir."] does not exist.</p>";   
  $uploadOk = 0;    
}

// Check if file already exists

if (file_exists($target_file)) {
  echo "<p>Désolé le fichier existe déjà.</p>";
  $uploadOk = 0;
}

// Check file size
if ($_FILES["file"]["size"] > 5*1048576) {
  echo "Désolé, le fichier est trop gros.";
  $uploadOk = 0;
}


// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Désolé votre fichier n'a pas pu être téléchargé.";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
    echo "<p>Le fichier <span style='color:red'>";
    echo htmlspecialchars( basename( $_FILES["file"]["name"])). "</span> a été téléchargé dans ";
    echo "<a href='img/download/'>Répertoire</a></p>";
  } else {
    echo "Désolé, il y a eu une erreur pendant le téléchargement. ";
    echo $_FILES["file"]["tmp_name"]." -> ".$target_file;
  }
}
?>
  </div>
  <hr>
			<h3><a href="/">retour vers répertoire racine.</a></h3>
	</body>
</html>
			