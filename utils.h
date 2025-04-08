#ifndef UTILS_H
#define UTILS_H

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

extern int channel_busy;

void generate_data(Device *device);
void transmit(Device *device);
void transmission_complete(Device *device);
int check_collision(Device devices[], int num_devices);
void handle_collision(Device devices[], int num_devices);
void backoff(Device devices[], int num_devices);
void initialize_devices(Device devices[], int num_devices);

#endif // UTILS_H