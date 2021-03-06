<?php
require_once 'Common.inc';
class Expiry extends CouchbaseTestCommon {

// The basic one replaces 009, the rest are new.

	function _skip_touch_common() {
		if (!$this->atLeastVersion(array(1,1))) {
			$this->markTestSkipped("Touch implemented in ext >= v1.1");
		}
	}

    /**
     * @test Expiration
     * @pre Set a key, with an expiry of 1. Get the key. Sleep two seconds,
     * get it again.
     *
     * @post First get should succeed, second should fail with @c ENOENT
     *
     * @test_plans{2.9.1}
     *
     * @group slow
     */
    function testExpirySetOO() {
        $oo = $this->oo;
        $key = $this->mk_key();

        $oo->add($key, "foo", 1);
        $this->assertEquals("foo", $oo->get($key));
        sleep(2);
        $this->assertNull($oo->get($key));
    }


    /**
     * @test Expiration (zero expiry)
     * @pre Store a kv with an expiry of @c 0. Wait two seconds, and retrieve
     * the value
     * @post Retrieval succeeds
     *
     * @test_plans{2.9.2}
     *
     * @group slow
     */
    function testExpirySetZeroOO() {
        $oo = $this->oo;
        $key = $this->mk_key();
        $oo->add($key, "foo", 0);
        sleep(2);
        $this->assertEquals("foo", $oo->get($key));
    }

    /**
     * @test Touch
     *
     * This is like @ref testExpirySetOO
     *
     * @group slow
     */
    function testExpiryTouch() {
        $oo = $this->oo;
        $key = $this->mk_key();
        $oo->set($key, "foo");
        $oo->touch($key, 1);

        $this->assertEquals("foo",
                            $oo->get($key),
                            "Key exists");

		sleep(2);

        $this->assertNull($oo->get($key));
    }

	function skipExpiryTouch() {
		$this->_skip_touch_common();
	}

    /**
     * @test Touch (zero expiry)
     * This is like @ref testExpirySetZeroOO
     * @group slow
     */
    function textExpiryTouchZero() {
        $oo = $this->oo;
        $key = $this->mk_key();
        $oo->set($key, "foo");
        $oo->touch($key, 0);

		sleep(2);

        $this->assertEquals("foo", $oo->get($key));
    }

    /**
     * @test Expiry (arithmetic)
     *
	 * Tests setting expiry time as part of increment/decrement operations on a numeric key.
	 *
	 * @group slow
     */
    function testArithmeticExpiry() {
		$oo = $this->oo;
		$key = $this->mk_key();

		$oo->set($key, 42);

		$oo->increment($key, 20, 0, 1, 0);
		$oo->decrement($key, 10, 0, 1, 0);

		sleep(2);

		$this->assertNull($oo->get($key));
    }

	/**
	 * @test Touch-Multi (expiry)
	 *
	 * This tests touching multiple items at once to set an expiry time.
	 *
	 * @group slow
	 */
	function testExpiryTouchMulti() {
		$this->skipExpiryTouchMulti();

		$oo = $this->oo;
		$keys = $this->makeKvPairs();

		foreach ($keys as $k => $v) {
			$oo->set($k, $v);
		}

		$oo->touchMulti(array_keys($keys), 1);

		sleep(2);

		$values = $oo->getMulti(array_keys($keys));

		$this->assertTrue(sizeof($values) == 0);
	}

	function skipExpiryTouchMulti() {
		$this->_skip_touch_common();
	}

    /**
     * @test Test negative expiry
     * @pre Set a key with a negative expiry
     * @post Error is raised
     * @test_plans{2.9.3}
     */
    function testNegativeExpirySet() {
        $oo = $this->getPersistOO();
        $k = $this->mk_key();
        $v = "a value";

        $msg = NULL;
        try {
            $rv = $oo->set($k, $v, -1);
        } catch (Exception $e) {
            $msg = $e->getMessage();
        }
        $this->assertRegExp('/^Expiry must not be negative.+/',$msg);
    }
}
