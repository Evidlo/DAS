<!DOCTYPE html>
<html>
<body>


<style media="screen" type="text/css">
body{
	margin:0;
	font-family:Terminus;
	background-color:black;
	padding:10px;
	font-size:25px;
}

.para-a{
	background-color:gray;
	padding:10px;
	margin:5px 0 0 0;
	border-radius:10px 0 0 10px;
	float:left;
}

.para-b{
	background-color:#EBEBEB;
	padding:10px;
	border-radius:0 10px 10px 0;
	float:left;
	margin:5px 0 0 0;
}

#doorstates{
	float:left;
}

#currstates{
	float:left;
	margin:20px;
	text-align:center;
}

.infobox{
	color:white;
	padding:18px;
	border-radius:10px;
	margin:10px;
}	
</style>


<div class="infobox">DAS Status Page</div>
<div id="doorstates">
	<table>
	<tr><td class="para-a">Timestamp</a></td>
	<td class="para-b">Command</a></td>
	</tr>
	<?php
	$doors_db = new PDO('sqlite:/home/evan/DAS/test.db');
	$result = $doors_db->query('SELECT * FROM DOORS');

	foreach($result as $row) {
    	echo "<tr style=\" \">";
    	echo "<td class=\"para-a\">",$row[0],"</td>";
    	echo "<td class=\"para-b\">",$row[1],"</td>";
    	echo "</tr>\n";
    	}
	?>

	</table>
</div>

<div id="currstates">
	<div class="infobox" style="background-color:gray">Door Status</div>
	<?php
	$db = new PDO('sqlite:/home/evan/DAS/test.db');
	$doors = $db->query('SELECT * FROM DOORS')->fetchALL();
	$door_open =  $doors[sizeof($doors)-1][1]; //Get last state of door
	
	echo "<div class=\"para-a\">Closed</div>";

	if ($door_open){
		$color="red";
		$text="No";
	}
	else {
		$color="green";
		$text="Yes";
	}
	echo "<div class=\"para-b\" style=\"color:white;background-color:$color\">$text</div>";

	$db = new PDO('sqlite:/home/evan/DAS/test.db');
	$controls = $db->query('SELECT * FROM Controls')->fetchALL();
	$door_locked =  $controls[sizeof($controls)-1][1]; //Get last state of door

	if ($door_locked == "00"){
		$color="red";
		$text="No";
	}
	else {
		$color="green";
		$text="Yes";
	}

	echo "<br>";
	echo "<div class=\"para-a\">Locked</div>";

	echo "<div class=\"para-b\" style=\"color:white;background-color:$color\">$text</div>";
	?>

</div>

</body>
</html>
