--TEST--
EmptyKey - EmptyKeySet

--SKIPIF--
<?php
include dirname(__FILE__)."/../../cbtestframework/cbtest-phpt-loader.inc";
couchbase_phpt_skipif("EmptyKey", "testEmptyKeySet");

--FILE--
<?php
include dirname(__FILE__)."/../../cbtestframework/cbtest-phpt-loader.inc";
couchbase_phpt_runtest("EmptyKey", "testEmptyKeySet");
--EXPECT--
PHP_COUCHBASE_OK