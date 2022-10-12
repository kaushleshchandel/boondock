<?php

$host = "ls-69c6e4270aeb585f18e40053fd69edca0efcf581.cwewddwaz3hm.us-east-2.rds.amazonaws.com"; /* Host name */
$user = "dbmasteruser"; /* User */
$password = "6)R&=1.E~>zge<#1nN98~oFw[XTFOh2,"; /* Password */
$dbname = "bdk"; /* Database name */

$con = mysqli_connect($host, $user, $password,$dbname);
// Check connection
if (!$con) {
 die("Connection failed: " . mysqli_connect_error());
} 

function add_new_message($mac, $target_file, $file_size)
{
    try {
        $pdo = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        // execute the stored procedure
        $sql = 'CALL add_message("' . $mac . '","' . $target_file . '",' . $file_size . ')';
        // call the stored procedure
        $q = $pdo->query($sql);
        $q->setFetchMode(PDO::FETCH_ASSOC);
    } catch (PDOException $e) {
        $errorMessage = "Database insert error:";
        die("Error occurred:" . $e->getMessage());
    }
}
