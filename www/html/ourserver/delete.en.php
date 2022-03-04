<?php
header_remove();
header('Content-Type: text/html');
header_remove('Content-Type');
?>
<!DOCTYPE html>
<html lang="en">
  
<head>
    <title>42-WEBSERV Delete</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Importing the jQuery -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <link href="css/style.css" rel="stylesheet">
</head>
<meta name="viewport" content=
          "width=device-width, initial-scale=1.0">
<script>
  
    function makeDELETErequest(fichier) {
        $.ajax({
            url: '/img/download/'+fichier,
            type: 'DELETE',
            success: function (result) {
                alert(fichier+" erased");
                location.reload(true);
            },
            error: function(result) {             
                alert("Can't erase "+fichier);
            }
        });
        
    }
</script>
  
<body>
    <table align="center" style="font-size:small;">
        <tbody>
    <?php
    $scandir = scandir("img/download/");
    
    foreach($scandir as $fichier){
      if(preg_match("#\.(jpg|jpeg|png|gif|bmp|tif|pdf|txt)$#",strtolower($fichier))){
            echo "<tr><th>$fichier</th>"."<td><button onclick=\"makeDELETErequest('".$fichier."')\">Erase</button></td></tr>";
      }
    }
    ?>
        </tbody>
    </table>
    <hr>
	<h3><a href="/">return to root directory.</a></h3>
</body>
  
</html>