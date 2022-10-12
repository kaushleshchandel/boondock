<?php

include "_config.php";
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

if (isset($_GET["did"])) {
    $deviceMac = $_GET["did"];

    //Get the KPI's
    try {

        // execute the stored procedure
        $sql = 'CALL get_device_settings( "' . $deviceMac . '" );';

 
        // call the stored procedure
        $q = $pdo->query($sql);
        $q->setFetchMode(PDO::FETCH_ASSOC);

        $r = $q->fetch();

        $count = $q->rowCount();
        $currentDateTime = date('Y-m-d H:i:s');;

        if ($count > 0) {
            $rx_enabled = $r['rx_enabled'];
            $tx_enabled = $r['tx_enabled'];
            $speaker_out = $r['speaker_out'];
            $silence = $r['silence'];
            $audio_trigger = $r['audio_trigger'];
            $volume = $r['volume'];
            $max_recording = $r['max_recording'];
            $min_recording = $r['min_recording'];

            
        } else {
            $rx_enabled = 1;
            $tx_enabled = 0;
            $speaker_out = 0;
            $silence = 3000;
            $audio_trigger = 50;
            $volume = 50;
            $max_recording = 10;
            $min_recording = 1;

        }
    } catch (PDOException $e) {
        echo "Error";
        die("Error occurred:" . $e->getMessage());
    }

    $data = ":";

    $data = $data . $rx_enabled  . ":";
    $data = $data . $tx_enabled  . ":";
    $data = $data . $speaker_out  . ":";
    $data = $data . $volume . ":";
    $data = $data . $audio_trigger . ":";
    $data = $data . $silence . ":";
    $data = $data . $min_recording . ":";
    $data = $data . $max_recording . ":";

    echo $data;
} else
    echo "Error";
