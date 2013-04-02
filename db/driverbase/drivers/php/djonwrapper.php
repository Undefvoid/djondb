<?php

/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

// Try to load our extension if it's not already loaded.
if (!extension_loaded('djonwrapper')) {
  if (strtolower(substr(PHP_OS, 0, 3)) === 'win') {
    if (!dl('php_djonwrapper.dll')) return;
  } else {
    // PHP_SHLIB_SUFFIX gives 'dylib' on MacOS X but modules are 'so'.
    if (PHP_SHLIB_SUFFIX === 'dylib') {
      if (!dl('djonwrapper.so')) return;
    } else {
      if (!dl('djonwrapper.'.PHP_SHLIB_SUFFIX)) return;
    }
  }
}



abstract class djonwrapper {
	const FO_NONE = 0;

	const FO_TOKEN = FO_TOKEN;

	const FO_AND = FO_AND;

	const FO_OR = FO_OR;

	const FO_EQUALS = FO_EQUALS;

	const FO_NOT_EQUALS = FO_NOT_EQUALS;

	const FO_PARENTESIS_OPEN = FO_PARENTESIS_OPEN;

	const FO_PARENTESIS_CLOSE = FO_PARENTESIS_CLOSE;

	const FO_NOT = FO_NOT;

	const FO_LESSTHAN = FO_LESSTHAN;

	const FO_LESSEQUALTHAN = FO_LESSEQUALTHAN;

	const FO_GREATERTHAN = FO_GREATERTHAN;

	const FO_GREATEREQUALTHAN = FO_GREATEREQUALTHAN;

	const ET_CONSTANT = 0;

	const ET_SIMPLE = ET_SIMPLE;

	const ET_BINARY = ET_BINARY;

	const ET_UNARY = ET_UNARY;

	static function parseFilterOperator($text) {
		return parseFilterOperator($text);
	}

	static function bson_splitSelect($select) {
		return bson_splitSelect($select);
	}

	static function bson_subselect($select,$elment) {
		return bson_subselect($select,$elment);
	}

	const SERVER_PORT = SERVER_PORT;
}

/* PHP Proxy Classes */
class BSONObjVectorPtr {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($n=null) {
		if (is_resource($n) && get_resource_type($n) === '_p_std__vectorT_BSONObj_p_t') {
			$this->_cPtr=$n;
			return;
		}
		switch (func_num_args()) {
		case 0: $this->_cPtr=new_BSONObjVectorPtr(); break;
		default: $this->_cPtr=new_BSONObjVectorPtr($n);
		}
	}

	function size() {
		return BSONObjVectorPtr_size($this->_cPtr);
	}

	function capacity() {
		return BSONObjVectorPtr_capacity($this->_cPtr);
	}

	function reserve($n) {
		BSONObjVectorPtr_reserve($this->_cPtr,$n);
	}

	function clear() {
		BSONObjVectorPtr_clear($this->_cPtr);
	}

	function push($x) {
		BSONObjVectorPtr_push($this->_cPtr,$x);
	}

	function is_empty() {
		return BSONObjVectorPtr_is_empty($this->_cPtr);
	}

	function pop() {
		$r=BSONObjVectorPtr_pop($this->_cPtr);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONObj($r);
		}
		return $r;
	}

	function get($i) {
		$r=BSONObjVectorPtr_get($this->_cPtr,$i);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONObj($r);
		}
		return $r;
	}

	function set($i,$val) {
		BSONObjVectorPtr_set($this->_cPtr,$i,$val);
	}
}

class StringVector {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($n=null) {
		if (is_resource($n) && get_resource_type($n) === '_p_std__vectorT_std__string_t') {
			$this->_cPtr=$n;
			return;
		}
		switch (func_num_args()) {
		case 0: $this->_cPtr=new_StringVector(); break;
		default: $this->_cPtr=new_StringVector($n);
		}
	}

	function size() {
		return StringVector_size($this->_cPtr);
	}

	function capacity() {
		return StringVector_capacity($this->_cPtr);
	}

	function reserve($n) {
		StringVector_reserve($this->_cPtr,$n);
	}

	function clear() {
		StringVector_clear($this->_cPtr);
	}

	function push($x) {
		StringVector_push($this->_cPtr,$x);
	}

	function is_empty() {
		return StringVector_is_empty($this->_cPtr);
	}

	function pop() {
		return StringVector_pop($this->_cPtr);
	}

	function get($i) {
		return StringVector_get($this->_cPtr,$i);
	}

	function set($i,$val) {
		StringVector_set($this->_cPtr,$i,$val);
	}
}

class BSONArrayObj {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($orig=null) {
		if (is_resource($orig) && get_resource_type($orig) === '_p_BSONArrayObj') {
			$this->_cPtr=$orig;
			return;
		}
		switch (func_num_args()) {
		case 0: $this->_cPtr=new_BSONArrayObj(); break;
		default: $this->_cPtr=new_BSONArrayObj($orig);
		}
	}

	function length() {
		return BSONArrayObj_length($this->_cPtr);
	}

	function add($obj) {
		BSONArrayObj_add($this->_cPtr,$obj);
	}

	function get($index) {
		$r=BSONArrayObj_get($this->_cPtr,$index);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONObj($r);
		}
		return $r;
	}

	function toChar() {
		return BSONArrayObj_toChar($this->_cPtr);
	}

	function select($select) {
		$r=BSONArrayObj_select($this->_cPtr,$select);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONArrayObj($r);
		}
		return $r;
	}

	function begin() {
		return BSONArrayObj_begin($this->_cPtr);
	}

	function end() {
		return BSONArrayObj_end($this->_cPtr);
	}
}

class BSONObj {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($orig=null) {
		if (is_resource($orig) && get_resource_type($orig) === '_p_BSONObj') {
			$this->_cPtr=$orig;
			return;
		}
		switch (func_num_args()) {
		case 0: $this->_cPtr=new_BSONObj(); break;
		default: $this->_cPtr=new_BSONObj($orig);
		}
	}

	function add($arg1,$arg2,$arg3=null) {
		switch (func_num_args()) {
		case 2: BSONObj_add($this->_cPtr,$arg1,$arg2); break;
		default: BSONObj_add($this->_cPtr,$arg1,$arg2,$arg3);
		}
	}

	function has($arg1) {
		return BSONObj_has($this->_cPtr,$arg1);
	}

	function getInt($arg1) {
		return BSONObj_getInt($this->_cPtr,$arg1);
	}

	function getDouble($arg1) {
		return BSONObj_getDouble($this->_cPtr,$arg1);
	}

	function getLong($arg1) {
		return BSONObj_getLong($this->_cPtr,$arg1);
	}

	function getString($arg1) {
		return BSONObj_getString($this->_cPtr,$arg1);
	}

	function getBSON($arg1) {
		$r=BSONObj_getBSON($this->_cPtr,$arg1);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONObj($r);
		}
		return $r;
	}

	function getBSONArray($arg1) {
		$r=BSONObj_getBSONArray($this->_cPtr,$arg1);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONArrayObj($r);
		}
		return $r;
	}

	function get($arg1) {
		return BSONObj_get($this->_cPtr,$arg1);
	}

	function getContent($arg1,$arg2=null) {
		switch (func_num_args()) {
		case 1: $r=BSONObj_getContent($this->_cPtr,$arg1); break;
		default: $r=BSONObj_getContent($this->_cPtr,$arg1,$arg2);
		}
		return $r;
	}

	function getXpath($xpath) {
		return BSONObj_getXpath($this->_cPtr,$xpath);
	}

	function select($sel) {
		$r=BSONObj_select($this->_cPtr,$sel);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONObj($r);
		}
		return $r;
	}

	function type($arg1) {
		return BSONObj_type($this->_cPtr,$arg1);
	}

	function toChar() {
		return BSONObj_toChar($this->_cPtr);
	}

	function begin() {
		return BSONObj_begin($this->_cPtr);
	}

	function end() {
		return BSONObj_end($this->_cPtr);
	}

	function length() {
		return BSONObj_length($this->_cPtr);
	}
}

class BSONParser {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_BSONParser') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_BSONParser();
	}

	static function parse($sbson) {
		$r=BSONParser_parse($sbson);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONObj($r);
		}
		return $r;
	}

	static function parseArray($sbson) {
		$r=BSONParser_parseArray($sbson);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new BSONArrayObj($r);
		}
		return $r;
	}
}

class ParseException {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($code_or_orig,$error=null) {
		if (is_resource($code_or_orig) && get_resource_type($code_or_orig) === '_p_ParseException') {
			$this->_cPtr=$code_or_orig;
			return;
		}
		switch (func_num_args()) {
		case 1: $this->_cPtr=new_ParseException($code_or_orig); break;
		default: $this->_cPtr=new_ParseException($code_or_orig,$error);
		}
	}

	function what() {
		return ParseException_what($this->_cPtr);
	}

	function errorCode() {
		return ParseException_errorCode($this->_cPtr);
	}
}

class DjondbConnection {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($host_or_orig,$port=null) {
		if (is_resource($host_or_orig) && get_resource_type($host_or_orig) === '_p_djondb__DjondbConnection') {
			$this->_cPtr=$host_or_orig;
			return;
		}
		switch (func_num_args()) {
		case 1: $this->_cPtr=new_DjondbConnection($host_or_orig); break;
		default: $this->_cPtr=new_DjondbConnection($host_or_orig,$port);
		}
	}

	function beginTransaction() {
		return DjondbConnection_beginTransaction($this->_cPtr);
	}

	function commitTransaction() {
		DjondbConnection_commitTransaction($this->_cPtr);
	}

	function rollbackTransaction() {
		DjondbConnection_rollbackTransaction($this->_cPtr);
	}

	function open() {
		return DjondbConnection_open($this->_cPtr);
	}

	function close() {
		DjondbConnection_close($this->_cPtr);
	}

	function internalClose() {
		DjondbConnection_internalClose($this->_cPtr);
	}

	function isOpen() {
		return DjondbConnection_isOpen($this->_cPtr);
	}

	function shutdown() {
		return DjondbConnection_shutdown($this->_cPtr);
	}

	function insert($db,$ns,$json_or_obj) {
		return DjondbConnection_insert($this->_cPtr,$db,$ns,$json_or_obj);
	}

	function findByKey($db,$ns,$select_or_id,$id=null) {
		switch (func_num_args()) {
		case 3: $r=DjondbConnection_findByKey($this->_cPtr,$db,$ns,$select_or_id); break;
		default: $r=DjondbConnection_findByKey($this->_cPtr,$db,$ns,$select_or_id,$id);
		}
		if (!is_resource($r)) return $r;
		switch (get_resource_type($r)) {
		case '_p_BSONObj': return new BSONObj($r);
		default: return new BSONObj($r);
		}
	}

	function find($db,$ns,$filter_or_select=null,$filter=null) {
		switch (func_num_args()) {
		case 2: $r=DjondbConnection_find($this->_cPtr,$db,$ns); break;
		case 3: $r=DjondbConnection_find($this->_cPtr,$db,$ns,$filter_or_select); break;
		default: $r=DjondbConnection_find($this->_cPtr,$db,$ns,$filter_or_select,$filter);
		}
		if (!is_resource($r)) return $r;
		switch (get_resource_type($r)) {
		case '_p_BSONArrayObj': return new BSONArrayObj($r);
		default: return new BSONArrayObj($r);
		}
	}

	function update($db,$ns,$json_or_bson) {
		return DjondbConnection_update($this->_cPtr,$db,$ns,$json_or_bson);
	}

	function remove($db,$ns,$id,$revision) {
		return DjondbConnection_remove($this->_cPtr,$db,$ns,$id,$revision);
	}

	function dropNamespace($db,$ns) {
		return DjondbConnection_dropNamespace($this->_cPtr,$db,$ns);
	}

	function dbs() {
		$r=DjondbConnection_dbs($this->_cPtr);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new StringVector($r);
		}
		return $r;
	}

	function namespaces($db) {
		$r=DjondbConnection_namespaces($this->_cPtr,$db);
		if (is_resource($r)) {
			$c=substr(get_resource_type($r), (strpos(get_resource_type($r), '__') ? strpos(get_resource_type($r), '__') + 2 : 3));
			if (class_exists($c)) return new $c($r);
			return new StringVector($r);
		}
		return $r;
	}

	function host() {
		return DjondbConnection_host($this->_cPtr);
	}
}

class ConnectionReference {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === '_references') return ConnectionReference__references_set($this->_cPtr,$value);
		if ($var === '_connection') return ConnectionReference__connection_set($this->_cPtr,$value);
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if (function_exists('ConnectionReference_'.$var.'_set')) return true;
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === '_connection') return new DjondbConnection(ConnectionReference__connection_get($this->_cPtr));
		if ($var === '_references') return ConnectionReference__references_get($this->_cPtr);
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_djondb__ConnectionReference') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_ConnectionReference();
	}
}

class DjondbConnectionManager {
	public $_cPtr=null;
	protected $_pData=array();

	function __set($var,$value) {
		if ($var === 'thisown') return swig_djonwrapper_alter_newobject($this->_cPtr,$value);
		$this->_pData[$var] = $value;
	}

	function __isset($var) {
		if ($var === 'thisown') return true;
		return array_key_exists($var, $this->_pData);
	}

	function __get($var) {
		if ($var === 'thisown') return swig_djonwrapper_get_newobject($this->_cPtr);
		return $this->_pData[$var];
	}

	function __construct($res=null) {
		if (is_resource($res) && get_resource_type($res) === '_p_djondb__DjondbConnectionManager') {
			$this->_cPtr=$res;
			return;
		}
		$this->_cPtr=new_DjondbConnectionManager();
	}

	static function getConnection($host,$port=null) {
		switch (func_num_args()) {
		case 1: $r=DjondbConnectionManager_getConnection($host); break;
		default: $r=DjondbConnectionManager_getConnection($host,$port);
		}
		if (!is_resource($r)) return $r;
		switch (get_resource_type($r)) {
		case '_p_djondb__DjondbConnection': return new DjondbConnection($r);
		default: return new DjondbConnection($r);
		}
	}

	static function releaseConnection($conn) {
		DjondbConnectionManager_releaseConnection($conn);
	}
}


?>
