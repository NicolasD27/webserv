<?php
	header_remove();
    header('Content-Type: text/html');
    header_remove('Content-Type');
?>
!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>42 - Webserv upload</title>
		<link href="css/style.css" rel="stylesheet">
	</head>
	<body>
    <div id="main">
		<h1>Upload ! </h1>
		<?php
$target_dir = $_SERVER['UPLOAD_DIR'];
$target_file = $target_dir."/". basename($_FILES["file"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
//echo $_FILES["file"]["tmp_name"]." -> ".$target_file;

if (!file_exists($target_dir) && !is_dir($target_dir)) 
{
    echo "<p>Sorry, The directory [".$target_dir."] does not exist.</p>";   
    $uploadOk = 0;    
}

// Check if file already exists

if (file_exists($target_file)) {
    echo "<p>Sorry, file already exists.</p>";
    $uploadOk = 0;
}

// Check file size
if ($_FILES["file"]["size"] > 2*1048576) {
    echo "Sorry, your file is too large.";
    $uploadOk = 0;
}


// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["file"]["tmp_name"], $target_file)) {
    echo "<p>The file <span style='color:red'>";
    echo htmlspecialchars( basename( $_FILES["file"]["name"])). "</span> has been uploaded in ";
    echo "<a href='img/download/'>Directory</a></p>";
  } else {
    echo "Sorry, there was an error uploading your file. ";
    echo $_FILES["file"]["tmp_name"]." -> ".$target_file;
  }
}
?>
  </div>
  <hr>
	<h3><a href="/">return to root directory.</a></h3>
	</body>
</html>
			