#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum
{
    IDLE,
    TRANSMITTING,
    WAITING
} DeviceState;

typedef struct
{
    int id;
    DeviceState state;
    int hasData;
    int backoff_counter;
} Device;

int channel_busy = 0;

void generate_data(Device *device)
{
    device->hasData = 1;
}

void transmit(Device *device)
{
    if (device->hasData && channel_busy == 0)
    {
        printf("Device %d starts transmitting.\n", device->id);
        channel_busy = 1;
        device->state = TRANSMITTING;
        device->hasData = 0;
    }
}

void transmission_complete(Device *device)
{
    printf("Device %d finished transmitting.\n", device->id);
    channel_busy = 0;
    device->state = IDLE;
}

int check_collision(Device devices[], int num_devices)
{
    int transmitting_count = 0;
    for (int i = 0; i < num_devices; i++)
    {
        if (devices[i].state == TRANSMITTING)
        {
            transmitting_count++;
        }
    }
    return transmitting_count > 1;
}

void handle_collision(Device devices[], int num_devices)
{
    if (channel_busy && check_collision(devices, num_devices))
    {
        printf("Collision detected!\n");
        for (int i = 0; i < num_devices; i++)
        {
            if (devices[i].state == TRANSMITTING)
            {
                devices[i].backoff_counter = rand() % 5 + 1;
                devices[i].state = WAITING;
                printf("Device %d is waiting for %d time units.\n", devices[i].id, devices[i].backoff_counter);
            }
        }
    }
}

void backoff(Device devices[], int num_devices)
{
    for (int i = 0; i < num_devices; i++)
    {
        if (devices[i].state == WAITING)
        {
            devices[i].backoff_counter--;
            if (devices[i].backoff_counter <= 0)
            {
                devices[i].state = IDLE;
                printf("Device %d is ready to transmit again.\n", devices[i].id);
            }
        }
    }
}

void one_persistent()
{
    srand(time(NULL));

    int num_devices = 3;
    Device devices[num_devices];
    int transmission_duration = 3; // Each transmission takes 3 time steps to complete

    // Initialize Devices
    for (int i = 0; i < num_devices; i++)
    {
        devices[i].id = i + 1;
        devices[i].state = IDLE;
        devices[i].hasData = 0;
        devices[i].backoff_counter = 0;
    }

    // Track how long each device has been transmitting
    int transmitting_time[num_devices];
    for (int i = 0; i < num_devices; i++)
    {
        transmitting_time[i] = 0;
    }

    // Simulate time steps
    for (int time = 0; time < 20; time++)
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

        // Devices with data attempt to transmit if channel is idle
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
                    printf("Device %d wants to transmit but channel is BUSY.\n", devices[i].id);
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
    }
}