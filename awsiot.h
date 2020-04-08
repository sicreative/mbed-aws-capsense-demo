/*
 * awsiot.h
 *
 *  Created on: 31 Mar 2020
 *      Author: sc lee
 * Licensed under the Apache License, Version 2.0
 *
 */

#ifndef AWSIOT_H_
#define AWSIOT_H_

#include "network.h"
#include <string>
using namespace std;
/**
 *
 * connect to awsiot endpoint
 *
 * @param NetworkInterface Interface of already connected Mbed Network
 * @return if 1 success
 */
int awsiot_connect( NetworkInterface* network );

/**
 * Add new message for publish to aws, the message will send periodically.
 *
 *
 * @param name The key of message, the value will be updated if the message already waiting for send.
 * @param value The message of the Key
 *
 */
void awsiot_add_message(string name, string value);

#endif /* AWSIOT_H_ */
