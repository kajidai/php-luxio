--TEST--
Check for luxio btree api
--SKIPIF--
<?php if (!extension_loaded("luxio")) print "skip"; ?>
--FILE--
<?php 
echo "luxio extension is available";
$a = new LuxIOBtree();
var_dump($a);
$a->open('test', LUX_DB_CREAT);
$ret = $a->put('hoge', 'hoge');
var_dump($ret);
$ret = $a->get('hoge');
var_dump($ret);
$ret = $a->del('hoge');
var_dump($ret);
$ret = $a->get('hoge');
var_dump($ret);
?>
--EXPECT--
luxio extension is availableobject(luxiobtree)#1 (1) {
  ["luxio"]=>
  resource(4) of type (luxio object)
}
bool(true)
string(4) "hoge"
bool(true)
bool(false)
