# Demo of Cypress CY8CKIT-062-WIFI-BT send capsense to AWS 

Demo for  element14  Connected Cloud Challenge 


## Quick Install 
1. Install mbed cli
2. Clone the source code and to this directory
3. mbed config root .
4. mbed deploy
5. update the aws_config.h for AWS cert and endpoint, update network.h for WIFI SSID and password.
6. mbed compile -t GCC_ARM -m CY8CKIT_062_WIFI_BT -f --sterm
5. place the following code in the main.c file.

###WIFI Driver issue under Mbed
Be notice, by reference of latest PSOC6 library, the Mbed
mbed-os/targets/TARGET_Cypress/TARGET_PSOC6/TARGET_CY8CKIT_062_WIFI_BT/SDIO_HOST/SDIO_HOST.c
may require update to following content to solve the problem of WIFI cannot be startup when boot.
```
void SDIO_SetSdClkFrequency(uint32_t u32SdClkFreqHz)
{
    uint16_t u16Div;
    /*
     * The UDB SDIO implemenation has a extra divider internally that divides the input clock to the UDB
     * by 2. The desired clock frequency is hence intentionally multiplied by 2 in order to get the required
     * SDIO operating frequency.
     */
    u16Div = Cy_SysClk_ClkPeriGetFrequency() / (2 * u32SdClkFreqHz);
    Cy_SysClk_PeriphSetDivider(SDIO_HOST_Internal_Clock_DIV_TYPE, SDIO_HOST_Internal_Clock_DIV_NUM, (u16Div-1));
}
```
