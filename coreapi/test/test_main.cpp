#ifdef UNIT_TEST

#include <Arduino.h>
#include <unity.h>

#include <coreapi.h>

void test_setup() {

}


void test_loop() {

}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_setup);

    
}


void loop() {

    RUN_TEST(test_loop);

    UNITY_END(); // stop unit testing
}


#endif // UNIT_TEST