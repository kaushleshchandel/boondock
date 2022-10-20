<?php

# This is the Server configuration file
# If you host your database and MQTT on same server, use localhost, or replace with the server

$mqtt_server   = 'localhost';
$mqtt_port     = 1883;
$mqtt_clientId = rand(5, 15);
$mqtt_username = 'boondock';
$mqtt_password = 'boon!dock';
$mqtt_clean_session = false;

$uploadFolder = "uploads/";

session_start();

$db_host = "localhost"; /* Host name */
$db_user = "boondock"; /* User */
$db_password = "boon!dock"; /* Password */
$dbname = "bdk"; /* Database name */

$pdo = new PDO("mysql:host=$db_host;dbname=$dbname", $db_user, $db_password);


$con = mysqli_connect($db_host, $db_user, $db_password,$dbname);
// Check connection
if (!$con) {
 die("Connection failed: " . mysqli_connect_error());
}
