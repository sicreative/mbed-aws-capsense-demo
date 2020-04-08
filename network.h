/*
 * network.h
 *
 * Handling Network activites
 *
 *      Author: sc lee
 *  Licensed under the Apache License, Version 2.0
 *
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "WhdSTAInterface.h"

/**
 *
 * Set WIFI SSID and WPA2 password
 *
 */
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

/**
 *
 * Initialization the network Interface
 *
 */
void initNetworkInterface();

/**
 *
 * Connect to network
 *
 */
void network_connect( void );

/**
 *
 * Disconnect to network
 */
void network_disconnect( void );

/**
 *
 *
 * Get Network Interface
 *
 * @return WhdSTAInterface* WHD WIFI Network interface
 *
 */
WhdSTAInterface* get_network_interface( void );



#endif /* NETWORK_H_ */
