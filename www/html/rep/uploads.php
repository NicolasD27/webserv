<?php
$target_dir = $_SERVER['UPLOAD_DIR'];
$target_file = $target_dir."/". basename($_FILES["avatar"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
//echo $_FILES["avatar"]["tmp_name"]." -> ".$target_file;

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
if ($_FILES["avatar"]["size"] > 500000) {
  echo "Sorry, your file is too large.";
  $uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
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