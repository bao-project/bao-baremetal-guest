#include <plat/init_platform.h>

void platform_custom_init(void)
{
    /*** Peripheral Bus Guard Initialization ***/
    pbg_init();

    /*** Module Standby Initialization ***/
    stbc_init();

    /*** MCU Clock Initialization ***/
    clock_init();

    /*** IO Ports Initialization ***/
    port_init();
}
