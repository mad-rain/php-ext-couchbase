--TEST--
Delete - DeleteNonExist

--SKIPIF--
<?php
include dirname(__FILE__)."/../../cbtestframework/cbtest-phpt-loader.inc";
couchbase_phpt_skipif("Delete", "testDeleteNonExist");

--FILE--
<?php
include dirname(__FILE__)."/../../cbtestframework/cbtest-phpt-loader.inc";
couchbase_phpt_runtest("Delete", "testDeleteNonExist");
--EXPECT--
PHP_COUCHBASE_OK