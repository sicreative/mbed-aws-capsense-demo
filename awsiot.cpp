/*
 * awsiot.cpp
 *
 *      Author: sc lee
 *
 *  Licensed under the Apache License, Version 2.0
 */

#include "awsiot.h"





/**
 *
 * Handling AWS Activities
 */
#include "mbed.h"
#include "aws_client.h"
#include "aws_config.h"

#include "lcd_ui.h"

#include <map>




NetworkInterface* _network;

void awsiot_send(void);



#define AWSIOT_KEEPALIVE_TIMEOUT            (600)
#define AWS_IOT_SECURE_PORT                 (8883)
#define AWSIOT_TIMEOUT_IN_USEC              (1000UL * 1000UL)
#define AWSIOT_SEND_PERIOD_MS                2000

DigitalOut led(LED1);


AWSIoTClient *client = NULL;
aws_publish_params_t publish_params;


Thread awsiot_thread(osPriorityNormal, 2048, NULL, "awsiot_send_thread");
Semaphore awsiot_sem(1);
EventQueue awiot_queue;

map<string,string> awsiot_send_map;



int awsiot_connect( NetworkInterface* network )
{




	aws_connect_params_t conn_params;
	aws_endpoint_params_t endpoint_params;

	if (!network)
		return -1;

    conn_params = { 0, 0, NULL, NULL, NULL, NULL, NULL };
    publish_params = { AWS_QOS_ATMOST_ONCE };
    endpoint_params = { AWS_TRANSPORT_MQTT_NATIVE, NULL, 0, NULL, 0 };
    client = NULL;


    cy_rslt_t result = CY_RSLT_SUCCESS;


    _network = network;




    if ( ( strlen(SSL_CLIENTKEY_PEM) | strlen(SSL_CLIENTCERT_PEM) | strlen(SSL_CA_PEM) ) < 64 )
    {
    	   lcd_msg("aws_config.h error",0);

        APP_INFO(( "Please configure SSL_CLIENTKEY_PEM, SSL_CLIENTCERT_PEM and SSL_CA_PEM in aws_config.h file\n" ));
        return -1;
    }

    /* Initialize AWS Client library */
    client = new AWSIoTClient( network, AWSIOT_THING_NAME, SSL_CLIENTKEY_PEM, strlen(SSL_CLIENTKEY_PEM), SSL_CLIENTCERT_PEM, strlen(SSL_CLIENTCERT_PEM) );

    /* set MQTT endpoint parameters */
    endpoint_params.transport = AWS_TRANSPORT_MQTT_NATIVE;
    endpoint_params.uri = (char *)AWSIOT_ENDPOINT_ADDRESS;
    endpoint_params.port = AWS_IOT_SECURE_PORT;
    endpoint_params.root_ca = SSL_CA_PEM;
    endpoint_params.root_ca_length = strlen(SSL_CA_PEM);

    /* set MQTT connection parameters */
    conn_params.username = NULL;
    conn_params.password = NULL;
    conn_params.keep_alive = AWSIOT_KEEPALIVE_TIMEOUT;
    conn_params.peer_cn = (uint8_t*)AWSIOT_ENDPOINT_ADDRESS;
    conn_params.client_id = (uint8_t*)AWSIOT_THING_NAME;

    /* connect to an AWS endpoint */
    result = client->connect( conn_params, endpoint_params );
    if ( result != CY_RSLT_SUCCESS )
    {
    	lcd_msg("connection AWS failed %d",0,result);
        APP_INFO(( "connection to AWS endpoint failed\r\n" ));
        if( client != NULL )
        {
            delete client;
            client = NULL;
        }
        return -1;
    }

    lcd_msg("connected to AWS ",0);
    APP_INFO(( "Connected to AWS endpoint\r\n" ));



    /*Start thread for send data batch to AWS period of time */
    awsiot_thread.start(callback(&awiot_queue, &EventQueue::dispatch_forever));
    awiot_queue.call_every(AWSIOT_SEND_PERIOD_MS, awsiot_send);



    return 1;
}

int awsiot_publish( const char* message){
    cy_rslt_t result = CY_RSLT_SUCCESS;
    publish_params.QoS = AWS_QOS_ATMOST_ONCE;

    	if (client==NULL)
    		return 1;

       result = client->publish( AWSIOT_TOPIC, message, strlen((char*)message), publish_params );
       if ( result != CY_RSLT_SUCCESS )
       {
    	   lcd_msg("publish to AWS failed %d ",2,result);
           APP_INFO(( "publish to topic failed\r\n" ));




           if( client != NULL )
           {
               delete client;
               client = NULL;
           }
           return 1;
       }

       client->yield();

       lcd_msg_delay_close("published to AWS",2,1000);
       APP_INFO(( "Published to topic %s",message ));


}



void awsiot_add_message(string name, string value){
	awsiot_sem.try_acquire();
	//awsiot_send_map.insert(pair<string,string>(name,value) );

	awsiot_send_map[name]=value;
	awsiot_sem.release();


}


void awsiot_send(){


	awsiot_sem.acquire();
	 if (awsiot_send_map.empty()){
		 printf("no message to send...\n");
		// awsiot_sem.release();
		 return;
	 }
	 string sendmessage = "{";

	 for (std::map<string,string>::iterator it=awsiot_send_map.begin(); it!=awsiot_send_map.end(); ++it){

		 if (it!=awsiot_send_map.begin())
			 sendmessage.append(",");

	 	sendmessage.append("\"");
	 	sendmessage.append(it->first);
	 	sendmessage.append("\":\"");
	 	sendmessage.append(it->second);
	 	sendmessage.append("\"");


	 }

	 sendmessage.append("}");

	 printf(sendmessage.c_str());
	 printf("\n");

	 awsiot_publish(sendmessage.c_str());

	 printf("published and reset map\n");

	 awsiot_send_map.clear();
	 awsiot_sem.release();

}

