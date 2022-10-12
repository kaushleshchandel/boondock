<?php

declare(strict_types=1);

$server   = 'MQTT_SERVER';
$port     = 1883;
$clientId = rand(5, 15);
$username = 'MQTT_USERNAME';
$password = 'MQTT_PASSWORD';
$clean_session = false;

require('vendor/autoload.php');

use \PhpMqtt\Client\MqttClient;
use \PhpMqtt\Client\ConnectionSettings;
 
use \PhpMqtt\Client\Exceptions\MqttClientException;
use \Psr\Log\LogLevel;


try {
    // Créez une nouvelle instance d'un client MQTT et configurez-la pour utiliser l'hôte et le port du courtier partagé.
    $client = new MqttClient($server, $port , 'test-publisher', MqttClient::MQTT_3_1, null);

    // Créez et configurez les paramètres de connexion selon les besoins.
    $connectionSettings = (new ConnectionSettings)
        ->setUsername($username)
        ->setPassword($password);

    // Connectez-vous au courtier avec les paramètres de connexion configurés et avec une session propre.
    $client->connect($connectionSettings, true);

    // S'abonner au sujet 'v3/@ttn/devices/eui-a86/up' en utilisant QoS 0.

    $client->publish("Hello", "World");
 
    // Termine gracieusement la connexion au courtier.
    $client->disconnect();
} catch (MqttClientException $e) {
    // MqttClientException est l'exception de base de toutes les exceptions de la bibliothèque. Si vous l'attrapez, vous attraperez toutes les exceptions liées à MQTT.
 
}

header("location: home.php");