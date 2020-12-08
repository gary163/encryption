<?php
$str = '杨凯风aaaabbbbccc oiej ';
$en = yi_encode($str);
var_dump($en);

$de = yi_decode($en);
var_dump($de);

