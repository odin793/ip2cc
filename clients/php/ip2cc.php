<?php

class IP2CC implements ArrayAccess {
  
  public $filename;
  
  public function __construct($filename) {
    $this->filename = $filename;
  }
  
  public function offsetGet($ip) {
    $fp = fopen($this->filename, 'rb');
    $parts = explode('.', $ip);
    $offset = 0;
    foreach($parts as $part) {
      $start = $offset + intval($part) * 4;
      fseek($fp, $start);
      $value = fread($fp, 4);
      if (substr($value, 0, 2) == "\xFF\xFF") {
        $res = substr($value, 2, 2);
        if ($res == "\x00\x00") {
          fclose($fp);
          return;
        }
        fclose($fp);
        return $res;
      }
      $chunks = unpack('N', $value);
      $offset = $chunks[1];
    }
    fclose($fp);
    throw new Exception('broken db');
  }
  
  public function offsetExists($foo) {
  }
  
  public function offsetUnset($bar) {
  }
  
  public function offsetSet($spam, $eggs) {
  }
  
}

if ($_SERVER['argc'] > 1 && realpath($_SERVER['argv'][0]) == __file__ ){
  
  $a = new IP2CC('../../ip2cc.db');
  $ip = $_SERVER['argv'][1];
  print "{$a[$ip]}\n";
  
}