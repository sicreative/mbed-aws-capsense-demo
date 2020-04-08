/*
 * network.cpp
 *
 *
 *  Author: sc  lee
 *
 * Licensed under the Apache License, Version 2.0
 *
 */

#include "network.h"

#include "lcd_ui.h"
#include "mbed.h"
#include "whdstainterface.h"






WhdSTAInterface  *network_interface;




void network_disconnect( void ){
	if (!network_interface)
		return;
	network_interface->disconnect();

	network_interface = NULL;
	return;
}


WhdSTAInterface* get_network_interface(){
	return network_interface;
}

void initNetworkInterface(){
	network_interface = new WhdSTAInterface();
//	network_interface->get_emac().power_down();
//	network_interface->get_emac().power_up();
//	network_interface->get_emac().power_down();


	if (!network_interface){
		APP_INFO(("ERROR: No WiFiInterface found.\n"));
		return;
	}


	//network_interface->scan(NULL,0);

	//network_interface->disconnect();

}


 void network_connect( void ){


	 if (!network_interface)
		 return;

    SocketAddress address;
    cy_rslt_t result = CY_RSLT_SUCCESS;
    //LCD_MSG("Connecting Wifi",0);


    if (network_interface->get_connection_status() != NSAPI_STATUS_DISCONNECTED)
        {
    	 APP_INFO(( "WIFI is connected\r\n" ));
            return;
        }


    APP_INFO(( "Connecting to the network using Wifi...\r\n" ));


   // network_interface->set_default_parameters();





    nsapi_error_t net_status = -1;
    for ( int tries = 0; tries < 3; tries++ )
    {



        int numofwifi = network_interface->scan(NULL, 0);

        if (numofwifi>10)
        	numofwifi=10;


        lcd_msg("num of wifi: %d",1,numofwifi);

        WiFiAccessPoint* wifipoint = new WiFiAccessPoint[numofwifi];



        network_interface->scan(wifipoint, numofwifi);

        for(int i=0;i<numofwifi;i++){
        	if (strcmp(wifipoint[i].get_ssid(),WIFI_SSID))
        			continue;

        	  network_interface->set_credentials(WIFI_SSID,WIFI_PASSWORD , NSAPI_SECURITY_WPA_WPA2);



        	  break;
        }






        net_status = network_interface->connect();
        if ( net_status == NSAPI_ERROR_OK )
        {

            break;
        }
        else
        {

        //	LCD_MSG("Retry Wifi %i",tries);
            APP_INFO(( "Unable to connect to network. Retrying...\r\n" ));
        }
    }

    if ( net_status != NSAPI_ERROR_OK )
    {
    	lcd_msg("No Wifi, %d", net_status);
        APP_INFO(( "ERROR: Connecting to the network failed (%d)!\r\n", net_status ));
        network_interface = NULL;
        return ;
    }

    network_interface->get_ip_address(&address);
    lcd_msg("IP: %s ",1,address.get_ip_address() );
    APP_INFO(( "Connected to the network successfully. IP address: %s\n", address.get_ip_address() ));

	return ;

}
