<?php

function human_readable_file_size($size)
{
    if ($size >= 1073741824) {
      $fileSize = round($size / 1024 / 1024 / 1024,1) . 'GB';
    } elseif ($size >= 1048576) {
        $fileSize = round($size / 1024 / 1024,1) . 'MB';
    } elseif($size >= 1024) {
        $fileSize = round($size / 1024,1) . 'KB';
    } else {
        $fileSize = $size . ' bytes';
    }
    return $fileSize;
}

function time_elapsed_string($datetime, $full = false) {
    $now = new DateTime;
    $ago = new DateTime($datetime);
    $diff = $now->diff($ago);

    $diff->w = floor($diff->d / 7);
    $diff->d -= $diff->w * 7;

    $string = array(
        'y' => 'year',
        'm' => 'month',
        'w' => 'week',
        'd' => 'day',
        'h' => 'hour',
        'i' => 'minute',
        's' => 'second',
    );
    foreach ($string as $k => &$v) {
        if ($diff->$k) {
            $v = $diff->$k . ' ' . $v . ($diff->$k > 1 ? 's' : '');
        } else {
            unset($string[$k]);
        }
    }

    if (!$full) $string = array_slice($string, 0, 1);
    return $string ? implode(', ', $string) . ' ago' : 'just now';
}
