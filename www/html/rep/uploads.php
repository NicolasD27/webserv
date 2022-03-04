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

$target_dir = $_SERVER['UPLOAD_DIR'];
$target_file = "";
if(isset($_FILES["avatar"]))
{
  $target_file = $target_dir."/". basename(utf8_decode($_FILES["avatar"]["name"]));
  $uploadOk = 1;
}
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
?>
<!DOCTYPE html>
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
if (!file_exists($target_dir) && !is_dir($target_dir)) 
{
  echo "<p>Sorry, The directory [".$target_dir."] does not exist.</p>";   
  $uploadOk = 0;    
}

// Check if file already exists

if (file_exists($target_file)) {
  echo "<p>Sorry, file already exists.</p>";
  // $uploadOk = 0;
}

// Check file size
if ($_SERVER["CONTENT_LENGTH"] > return_bytes( ini_get('upload_max_filesize'))) {
  echo "Sorry, your file is too large.</br>upload_max_filesize (php.ini) = ".ini_get('upload_max_filesize')."</p>";
  $uploadOk = 0;
}

// Allow certain file formats
if($uploadOk && $imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" && $imageFileType != "bmp") {
  echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
  $uploadOk = 0;
}


// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["avatar"]["tmp_name"], $target_file)) {
    echo "<p>The file ". htmlspecialchars( basename( $_FILES["avatar"]["name"])). " has been uploaded.</p>";
  } else {
    echo "Sorry, there was an error uploading your file. ";
    echo $_FILES["avatar"]["tmp_name"]." -> ".$target_file;
  }
}
?>
</div>
</body>
</html>
    