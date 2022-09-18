<?php

include "_config.php";

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);


$target_dir = "uploads/" . basename($_FILES["audioFile"]["name"]) . "/";
$datum = mktime(date('H') + 0, date('i'), date('s'), date('m'), date('d'), date('y'));
$target_file = $target_dir . date('Y_m_d_H_i_s', $datum) . ".wav";
$uploadOk = 1;
$audioFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));

$directoryName = "uploads/" . basename($_FILES["audioFile"]["name"]);

$mac =  basename($_FILES["audioFile"]["name"]);

$errorMessage = "";

// Check if file already exists
if (!file_exists($directoryName)) {
  mkdir($directoryName);
}


// Check if audio file is a actual audio or fake audio


// Check if file already exists
if (file_exists($target_file)) {
  $errorMessage = "File exists:";
  $uploadOk = 0;
}

// Check file size
if ($_FILES["audioFile"]["size"] > 50000000) {
  $errorMessage = "File Large:";
  $uploadOk = 0;
}

// Allow certain file formats
if (
  $audioFileType != "wav" && $audioFileType != "mp3" && $audioFileType != "raw"
  && $audioFileType != "wma"
) {

  $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  //echo "ERR0";
  // if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["audioFile"]["tmp_name"], $target_file)) {
    $errorMessage = "OK";

    //Add to database
    echo $target_file;

    try {
      $pdo = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
      // execute the stored procedure
      $sql = 'CALL add_message("' . $mac . '","' . $target_file . '",' . $_FILES["audioFile"]["size"] . ')';
      //echo $sql;
      // call the stored procedure
      $q = $pdo->query($sql);
      $q->setFetchMode(PDO::FETCH_ASSOC);
    } catch (PDOException $e) {
      $errorMessage = "Database insert error:";
      die("Error occurred:" . $e->getMessage());
    }
  } else {
    $errorMessage = "Error Moving file:";
  }
}

$currentDateTime = date('Y-m-d H:i:s');;
$data = array("time" => "$currentDateTime", "retu" => "$errorMessage", "flas" => "1", "qual" => "5", "dela" => "60", "rebo" => "0", "comm" => " ");
$resp = json_encode($data);
echo $resp;
