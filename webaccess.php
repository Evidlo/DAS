<!DOCTYPE html>
<html>
<head>
<style media="screen" type="text/css">
body{
	font-family:Terminus;
	background-color:black;
	font-size:25px;
	padding:30px;
	text-align:center;
	margin:0 auto;
}

.infobox{
	color:white;
	border-radius:10px;
	margin:20px;
}

.para-a{
	background-color:gray;
	border-radius:10px 0 0 10px;
	text-align:center;
	display:inline-block;
}

.para-b{
	background-color:#EBEBEB;
	border-radius:0 10px 10px 0;
	display:inline-block;
}

#doorstates{
	border-collapse:collapse;
	display:inline-block;
}

#doorstates td{
	border: 1px solid black;
	background-color:gray;
	display:table-cell;
}

#doorstates .infobox{
	border-radius:0;
}

#currstates{
	width:400px;
	display:inline-block;
	vertical-align:top;
	font-size:32px;
}

#currstates .para-a{
	margin:10px 0 0 10px;
}

#currstates *{
	padding:15px;
}

table{
}

</style>
</head>


<body>
	<table id="doorstates">
	<div class="infobox">DAS Status Page</div>
		<tr><td class="infobox">Timestamp</a></td><td class="infobox">Command</a></td>
		</tr>
		<?php
		$doors_db = new PDO('sqlite:/home/evan/DAS/test.db');
		$result = $doors_db->query('SELECT * FROM DOORS');

		foreach($result as $row) {
		
			if ($row[1] == "1"){
				$door_state="Open";
				$color="red";
			}
			elseif($row[1] == "0"){
				$door_state="Closed";
				$color="green";
			}
			else{
				$door_state="Error";
				$color="Yellow";
			}
			
			echo "<tr>";
			echo "<td>",$row[0],"</td>";
			echo "<td style=\"background-color:$color;\">",$door_state,"</td>";
			echo "</tr>\n";
			}
		?>

	</table>
	
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

		echo "<br>";
		
		$db = new PDO('sqlite:/home/evan/DAS/test.db');
		$controls = $db->query('SELECT * FROM Controls')->fetchALL();
		$door_locked =  $controls[sizeof($controls)-1][1]; //Get last state of door

		if ($door_locked == "00"){
			$color="red";
			$text="No";
		}
		if ($door_locked == "01"){
			$color="green";
			$text="Yes";
		}

		echo "<div class=\"para-a\">Locked</div>";

		echo "<div class=\"para-b\" style=\"color:white;background-color:$color\">$text</div>";
		?>

		</div>
</body>
</html>
