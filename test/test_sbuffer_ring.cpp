#include "Catch/catch.hpp"

#include "../src/ringbuffer.c"


TEST_CASE("RINGBUFFER") {

    ringbuffer_t *buffer;

    SECTION("insert to ring buffer adds element") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 1);
        ringbuffer_insert(buffer, 20);

        REQUIRE(buffer->ring[0] == 1);
        REQUIRE(buffer->ring[1] == 20);

        ringbuffer_free(&buffer);
    }

    SECTION("insert to ring buffer increments producer index") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 11);
        ringbuffer_insert(buffer, 2);

        REQUIRE(buffer->producer == 2);

        ringbuffer_free(&buffer);
    }

    SECTION("peek gets next element for particular consumer with index") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 78);
        ringbuffer_insert(buffer, 9);

        uint8_t actual = 0;
        REQUIRE(ringbuffer_peek(buffer, &actual, 1) == RINGBUFFER_SUCCESS);
        REQUIRE(actual == 78);
        REQUIRE(ringbuffer_peek(buffer, &actual, 1) == RINGBUFFER_SUCCESS);
        REQUIRE(actual == 9);
        REQUIRE(ringbuffer_peek(buffer, &actual, 0) == RINGBUFFER_SUCCESS);
        REQUIRE(actual == 78);

        ringbuffer_free(&buffer);
    }

    SECTION("peek increments consumer index") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 78);
        ringbuffer_insert(buffer, 9);
        uint8_t actual = 0;
        ringbuffer_peek(buffer, &actual, 1);
        ringbuffer_peek(buffer, &actual, 1);
        ringbuffer_peek(buffer, &actual, 0);

        REQUIRE(buffer->consumers[1] == 2);
        REQUIRE(buffer->consumers[0] == 1);

        ringbuffer_free(&buffer);
    }

    SECTION("peek does not overtake producer index") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 5);
        uint8_t actual = 0;
        ringbuffer_peek(buffer, &actual, 1);
        ringbuffer_peek(buffer, &actual, 1);

        REQUIRE(actual == 5);
        REQUIRE(buffer->consumers[1] == 1);

        ringbuffer_free(&buffer);
    }

    SECTION("wraps and overwrites data when full") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 78);
        ringbuffer_insert(buffer, 9);
        uint8_t actual = 0;
        ringbuffer_peek(buffer, &actual, 1);
        ringbuffer_peek(buffer, &actual, 1);
        ringbuffer_peek(buffer, &actual, 0);
        ringbuffer_peek(buffer, &actual, 0);

        for (int i = 0; i < RINGBUFFER_SIZE; i++) {
            ringbuffer_insert(buffer, 99);
        }

        REQUIRE(buffer->ring[0] == 99);

        ringbuffer_free(&buffer);
    }

    SECTION("producer does not overtake consumers") {
        ringbuffer_init(&buffer);
        ringbuffer_insert(buffer, 2);
        for (int i = 0; i < RINGBUFFER_SIZE + 3; i++) {
            ringbuffer_insert(buffer, 45);
        }

        REQUIRE(buffer->producer == 55); // consumers are at 0, producer should be at the end
        REQUIRE(buffer->ring[0] == 2);

        ringbuffer_free(&buffer);
    }

}
