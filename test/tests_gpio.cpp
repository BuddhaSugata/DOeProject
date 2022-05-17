#include <cath2.h>

TEST("Gpio functionality")
{
    SECTION("Test if firmware throws an error if GPIO module is not available.")
    {
        MockGpio.return(false);
        REQUIRE(set_gpio(0, true) == false);
    }
    SECTION("Test if firmware returns true if everything is ok")
    {
        MockGpio.return(true);
        REQUIRE(set_gpio(0, true) == true);
    }
}