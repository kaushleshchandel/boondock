<?php
declare(strict_types=1);
include "_config.php";


if (isset($_GET["mid"]))
$messageLink = $_GET["mid"];

require('vendor/autoload.php');

use \PhpMqtt\Client\MqttClient;
use \PhpMqtt\Client\ConnectionSettings;
 
use \PhpMqtt\Client\Exceptions\MqttClientException;
use \Psr\Log\LogLevel;


try {
    // Créez une nouvelle instance d'un client MQTT et configurez-la pour utiliser l'hôte et le port du courtier partagé.
    $client = new MqttClient($mqtt_server, $mqtt_port , 'test-publisher', MqttClient::MQTT_3_1, null);

    // Créez et configurez les paramètres de connexion selon les besoins.
    $connectionSettings = (new ConnectionSettings)
        ->setUsername($mqtt_username)
        ->setPassword($mqtt_password);

    // Connectez-vous au courtier avec les paramètres de connexion configurés et avec une session propre.
    $client->connect($connectionSettings, true);

    // S'abonner au sujet 'v3/@ttn/devices/eui-a86/up' en utilisant QoS 0.


    try {
         
        // execute the stored procedure
        $sql = 'CALL get_tx_docks(' . $_SESSION["id"] . ')';
    
        // call the stored procedure
        $q = $pdo->query($sql);
        $q->setFetchMode(PDO::FETCH_ASSOC);
    } catch (PDOException $e) {
        die("Error occurred:" . $e->getMessage());
    }

    while ($r = $q->fetch()) :

        $client->publish($r['mac'] . '/set/play' ,  $messageLink);

    endwhile;

 
    // Termine gracieusement la connexion au courtier.
    $client->disconnect();
} catch (MqttClientException $e) {
    // MqttClientException est l'exception de base de toutes les exceptions de la bibliothèque. Si vous l'attrapez, vous attraperez toutes les exceptions liées à MQTT.
 
}

header("location: home.php");