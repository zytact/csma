#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

void non_persistent()
{
    srand(time(NULL));

    int num_devices = 3;
    Device devices[num_devices];
    int transmission_duration = 3; // Each transmission takes 3 time steps to complete

    initialize_devices(devices, num_devices);

    // Track how long each device has been transmitting
    int transmitting_time[num_devices];
    for (int i = 0; i < num_devices; i++)
    {
        transmitting_time[i] = 0;
    }

    // Simulate time steps
    for (int time = 1; time < 21; time++)
    {
        printf("\n--- Time Step %d ---\n", time);
        printf("Channel status: %s\n", channel_busy ? "BUSY" : "IDLE");

        // Randomly generate data for devices
        if (rand() % 3 == 0)
        {
            int device_id = rand() % num_devices;
            if (!devices[device_id].hasData && devices[device_id].state == IDLE)
            {
                generate_data(&devices[device_id]);
                printf("Device %d has data to send.\n", devices[device_id].id);
            }
        }

        // Devices with data attempt to transmit if channel is idle (non-persistent)
        for (int i = 0; i < num_devices; i++)
        {
            if (devices[i].hasData && devices[i].state == IDLE)
            {
                if (!channel_busy)
                {
                    transmit(&devices[i]);
                    if (devices[i].state == TRANSMITTING)
                    {
                        transmitting_time[i] = 1; // Start counting transmission time
                    }
                }
                else
                {
                    // In non-persistent, immediately go to backoff if channel is busy
                    devices[i].backoff_counter = rand() % 5 + 1;
                    devices[i].state = WAITING;
                    printf("Device %d found channel busy, backing off for %d time units.\n",
                           devices[i].id, devices[i].backoff_counter);
                }
            }
        }

        handle_collision(devices, num_devices);

        // Process ongoing transmissions
        for (int i = 0; i < num_devices; i++)
        {
            if (devices[i].state == TRANSMITTING)
            {
                // Check if transmission is complete
                if (transmitting_time[i] >= transmission_duration)
                {
                    transmission_complete(&devices[i]);
                    transmitting_time[i] = 0;
                }
                else
                {
                    printf("Device %d is transmitting (%d/%d).\n",
                           devices[i].id, transmitting_time[i], transmission_duration);
                    transmitting_time[i]++;
                }
            }
        }

        backoff(devices, num_devices);
        sleep(1);
    }
}
