/*
 *      Author: sc lee
 *
 *
 *      Demo of capture capsense button and publised data to AWS server
 *
 *      Inorder to run this demo, you should have CY8CKIT-062-WIFI-BT+TFT with CY8CKIT-028-TFT
 *
 *      Modifiy the aws_config.h before run
 *
 *		Detail:
 *
 *
 * Licensed under the Apache License, Version 2.0
 */




#include "mbed.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "GUI.h"
#include "network.h"
#include "awsiot.h"
#include "mbed_memory_status.h"
#include "capsense.h"
#include "lcd_ui.h"
#include "cy_smif.h"
#include "cy_smif_memslot.h"
#include "cycfg_qspi_memslot.h"
#include "qspi_api.h"

//#define HW_STATS

#if defined(HW_STATS)
#if !defined(MBED_CPU_STATS_ENABLED) || !defined(DEVICE_LPTICKER) || !defined(DEVICE_SLEEP)
#error [NOT_SUPPORTED] test not supported
#endif


#define HW_STATS_MAX_THREAD_STACK        384
#define HW_STATS_SAMPLE_TIME             5000    // msec


uint64_t prev_idle_time = 0;
int32_t wait_time = 20000;



/* QSPI HAL object */
static qspi_t QSPI_OBJ;


#pragma long_calls
void function_external_memory ();
#pragma long_calls_off






void calc_cpu_usage()
{
    mbed_stats_cpu_t stats;
    mbed_stats_cpu_get(&stats);

    uint64_t diff = (stats.idle_time - prev_idle_time);
    uint8_t idle = (diff * 100) / (HW_STATS_SAMPLE_TIME*1000);    // usec;
    uint8_t usage = 100 - ((diff * 100) / (HW_STATS_SAMPLE_TIME*1000));    // usec;;
    prev_idle_time = stats.idle_time;

    printf("Idle: %d Usage: %d \n", idle, usage);


}


void print_memory_info() {
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));

    cnt = mbed_stats_stack_get_each(stats, cnt);
    for (int i = 0; i < cnt; i++) {
        printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stats[i].max_size, stats[i].reserved_size);
    }
    free(stats);

    // Grab the heap statistics
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    printf("Heap size: %lu / %lu bytes\r\n", heap_stats.current_size, heap_stats.reserved_size);
}

void hw_stats(){
	calc_cpu_usage();
	print_memory_info();
}

void busy_thread()
{
    volatile uint64_t i = ~0;

    while(i--) {

    	ThisThread::sleep_for(wait_time);
    }
}


#endif





// Blinking rate in milliseconds
#define BLINKING_RATE_MS     500



int main()
{


#if defined(HW_STATS)

	   EventQueue *stats_queue = mbed_event_queue();
	    Thread *thread;
	    int id;

	    id = stats_queue->call_every(HW_STATS_SAMPLE_TIME, hw_stats);
	    thread = new Thread(osPriorityNormal, HW_STATS_MAX_THREAD_STACK);
	    thread->start(busy_thread);
#endif










   initNetworkInterface();

   lcd_StartUp();

   capsense_init();


   lcd_msg("START DEVICES...",0);

   lcd_msg_delay_close("Mbed: %d.%d.%d",2,1000,MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);











    if (!get_network_interface())
   	 return 1;



  network_connect();







  if (!awsiot_connect(get_network_interface()))
	  return -1;



 wait_us(osWaitForever);




}
