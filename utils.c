#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

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

void initialize_devices(Device devices[], int num_devices)
{
    for (int i = 0; i < num_devices; i++)
    {
        devices[i].id = i + 1;
        devices[i].state = IDLE;
        devices[i].hasData = 0;
        devices[i].backoff_counter = 0;
    }
}