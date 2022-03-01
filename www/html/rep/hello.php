<?php
	header_remove();
	header('Content-Type: text/html');
	header_remove('Content-Type');
	
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title>Hello</title>
		<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css" integrity="sha384-GJzZqFGwb1QTTN6wy59ffF1BuGJpLSa9DkKMp0DgiMDm4iYMj70gZWKYbI706tWS" crossorigin="anonymous">
		<!-- <link rel="stylesheet" href="index.css"> -->
	</head>
	<body>
		<h1><?php
				echo "Hello in php";
			?>
		</h1>
		<b><?php
		$date = date('d-m-y h:i:s');
		echo $date;
		?></b>
	</body>
</html>
