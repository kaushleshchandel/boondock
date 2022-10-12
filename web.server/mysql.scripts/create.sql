CREATE TABLE `dock` (
  `id` int NOT NULL AUTO_INCREMENT,
  `mac` varchar(45) DEFAULT NULL,
  `name` varchar(45) DEFAULT NULL,
  `sw_version` varchar(5) DEFAULT NULL,
  `hw_version` varchar(10) DEFAULT NULL,
  `last_online` datetime DEFAULT NULL,
  `is_online` int DEFAULT NULL,
  `owner` int DEFAULT NULL,
  `active` int DEFAULT NULL,
  `created` datetime DEFAULT CURRENT_TIMESTAMP,
  `updated` datetime DEFAULT CURRENT_TIMESTAMP,
  `last_seen` datetime DEFAULT CURRENT_TIMESTAMP,
  `station` varchar(45) DEFAULT NULL,
  `frequency` float DEFAULT NULL,
  `rx_enabled` int DEFAULT '0',
  `tx_enabled` int DEFAULT '0',
  `setting_speaker_out` int DEFAULT '0',
  `setting_silence` int DEFAULT '3000',
  `setting_audio_trigger` int DEFAULT '50',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `dock_pack` (
  `id` int NOT NULL AUTO_INCREMENT,
  `dock_id` int DEFAULT NULL,
  `user_id` int DEFAULT NULL,
  `pack_id` int DEFAULT NULL,
  `active` int DEFAULT NULL,
  `rx` varchar(2) DEFAULT NULL,
  `tx` varchar(2) DEFAULT NULL,
  `name` varchar(45) DEFAULT NULL,
  `created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `lat` float DEFAULT NULL,
  `lon` float DEFAULT NULL,
  `frequency` float DEFAULT NULL,
  `Station` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
CREATE TABLE `login` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `password` varchar(45) DEFAULT NULL,
  `created` datetime DEFAULT NULL,
  `active` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `dock_pack` (
  `id` int NOT NULL AUTO_INCREMENT,
  `dock_id` int DEFAULT NULL,
  `user_id` int DEFAULT NULL,
  `pack_id` int DEFAULT NULL,
  `active` int DEFAULT NULL,
  `rx` varchar(2) DEFAULT NULL,
  `tx` varchar(2) DEFAULT NULL,
  `name` varchar(45) DEFAULT NULL,
  `created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `lat` float DEFAULT NULL,
  `lon` float DEFAULT NULL,
  `frequency` float DEFAULT NULL,
  `Station` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `messages` (
  `id` int NOT NULL AUTO_INCREMENT,
  `mac` varchar(45) DEFAULT NULL,
  `length` int DEFAULT NULL,
  `file_name` varchar(45) DEFAULT NULL,
  `added` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `sent` int DEFAULT NULL,
  `queued` int DEFAULT '0',
  `station` varchar(45) DEFAULT NULL,
  `frequency` float DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1699 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `pack` (
  `id` int NOT NULL,
  `name` varchar(45) DEFAULT NULL,
  `active` int DEFAULT NULL,
  `created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `user` (
  `id` int NOT NULL AUTO_INCREMENT,
  `email` varchar(100) DEFAULT NULL,
  `username` varchar(100) NOT NULL,
  `firstname` varchar(45) NOT NULL,
  `lastname` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `active` int DEFAULT NULL,
  `created` datetime DEFAULT CURRENT_TIMESTAMP,
  `updated` datetime DEFAULT CURRENT_TIMESTAMP,
  `is_super_admin` int DEFAULT '0',
  `is_admin` int DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

DELIMITER $$
CREATE DEFINER=`dbmasteruser`@`%` PROCEDURE `add_message`(
IN MAC VARCHAR(20),
IN FILENAME VARCHAR(100),
IN MESSAGE_LENGTH INT
)
BEGIN
declare DOCK_STATION VARCHAR(50);
declare DOCK_FREQUENCY FLOAT;
 select station into DOCK_STATION from dock where dock.mac = MAC;
 select frequency into DOCK_FREQUENCY from dock where dock.mac = MAC;
 
 insert into messages(mac, length, file_name, sent, station, frequency)
 values ( MAC, MESSAGE_LENGTH, FILENAME,  0, DOCK_STATION, DOCK_FREQUENCY);
  
END$$
DELIMITER ;

DELIMITER $$
CREATE DEFINER=`dbmasteruser`@`%` PROCEDURE `get_active_messages`(
IN USERID INT,
IN DOCK_ID INT,
IN DOCK_PACK_ID INT
)
BEGIN

select 
messages.id, 
dock.id as dock_id, 
dock.name as name, 
messages.station, 
messages.frequency,
messages.mac, 
messages.length, 
messages.file_name,
messages.added,
messages.queued,
messages.sent,
dock.owner as owner
from messages 
left join dock on messages.mac = dock.mac
where dock.owner = USERID
order by messages.added desc
LIMIT 10;

END$$
DELIMITER ;

DELIMITER $$
CREATE DEFINER=`dbmasteruser`@`%` PROCEDURE `get_kpi`(
IN USERID INT
)
BEGIN

select 
count(distinct(dock.mac)) dock_count,
count(messages.id) total_messages,
COUNT(DISTINCT IF(messages.sent = 1,
            messages.sent,
            NULL)) AS sent_messages,
  (COUNT(DISTINCT IF(messages.sent = 1,
            messages.sent,
            NULL)))/count(messages.id)*100 AS retransmission_rate 
from messages 
left join dock on messages.mac = dock.mac
where dock.owner = USERID
order by messages.added desc;

END$$
DELIMITER ;
 
 DELIMITER $$
CREATE DEFINER=`dbmasteruser`@`%` PROCEDURE `get_user_docks`(
IN USERID INT
)
BEGIN
select id, mac, name,last_seen,  station,frequency, 
(select count(*) from messages where messages.mac = dock.mac) received,
(select count(*) from messages where messages.mac = dock.mac and sent =1) sent 
from dock 
where dock.owner = USERID;

END$$
DELIMITER ;
