<?php
  $first = $_GET['int1'];
  $second= $_GET['int2'];
  $sum = 0;
  for ($i = $first; $i <= $second; $i++){
    if ( $i % 2){
      $sum = $sum + $i;
    }
  }
?>
<html>
  <head>
    <title>PHP Form Processing</title>
    <style>
      div{width: 200px; height: 35px; margin-left: 30px;}
    </style>
  </head>
  <body>
    <h1>Sum of odd number within a given range</h1>
    <div>
   		First Integer: <?php echo $first; ?>
	</div>
	<div>
        Second Integer: <?php echo $second; ?>
    </div>
    <div style="font-weight: bold; border: solid 1px #444; padding: 6px; text-align:center; height: auto;">
    	Result: <?php echo $sum; ?>
    </div>
  </body>
</html>