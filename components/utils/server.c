/**
 * @file server.c
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-06-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "server.h"

int8_t server_init(system_defs **sys)

{
    system_defs *sys_def;
    int addr_family = 0;
    int ip_protocol = 0;
    ip_protocol = IPPROTO_IP;
    addr_family = AF_INET;

    int sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (sock < 0)
    {
        ESP_LOGE(TAG_SERVER, "Unable to create socket: errno %d", errno);
        return -1;
    }

    sys_def = (system_defs *)malloc(sizeof(*sys_def));
    if (!sys_def)
        return -1;
    /** Assign the newly created socket to the sys struct */
    sys_def->socket = sock;
    *sys = sys_def;
    return 0;
}

int8_t server_connect(system_defs *sys)
{

    char host_ip[] = HOST_IP_ADDR;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

    ESP_LOGI(TAG_SERVER, "Connecting to %s:%d", host_ip, PORT);

    int err = connect(sys->socket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
    if (err != 0)
    {
        ESP_LOGE(TAG_SERVER, "Socket unable to connect: errno %d", errno);
        return -2;
    }

    ESP_LOGI(TAG_SERVER, "Successfully connected");
    return 0;
}

/**
  @brief Load the Config from TCP server.
         - Loading Config for this specific device 
         - Sample rate for ADXL and Microphone. 
         - It will be stored in the system defines struct. 
 
  @param sys System define struct - Used to save the config. 
  @return int8_t 
 */
int8_t server_load_config(system_defs *sys)
{
    char rx_buffer[300];
    ESP_LOGE(TAG_SERVER, "Asking for config");
    const char *load = "\x01";
    char *json_string = json_create_config_string(sys, 0);
    /** Create string for tcp server with SOF and EOF */
    strncpy(rx_buffer, load, sizeof(rx_buffer));
    strncat(rx_buffer, json_string, sizeof(rx_buffer - load));
    strncat(rx_buffer, EndOfTrans, 50);
    int err = send(sys->socket, rx_buffer, sizeof(rx_buffer), 0);
    if (err < 0)
    {
        ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
        return -2;
    }
    int len = recv(sys->socket, rx_buffer, sizeof(rx_buffer) - 1, 0);
    // Error occurred during receiving
    if (len < 0)
    {
        ESP_LOGE(TAG_SERVER, "recv failed: errno %d", errno);
        return -1;
    }
    /** Null terminate string that is not used */
    rx_buffer[len] = 0;
    ESP_LOGI(TAG_SERVER, "Received: %s", rx_buffer);

    json_load_config(rx_buffer, sys);
    return 0;
}

int8_t server_disconnect(system_defs *sys)

{
    ESP_LOGI(TAG_SERVER, "Closing the socket");
    close(sys->socket);
    sys->socket = 0;
    return 0;
}

int8_t server_send_config(system_defs *sys)
{

    char *json_string = NULL;
    return -1;
}

/**
 * @brief 
 * 
 * @param sys System defines struct pointer
 * @param data_raw Raw data sampled
 * @param reads The amount of reads done (ADXL typ. 3200 and MIC 96k)
 * @param data_x Sampled X-axis accel data
 * @param data_y Sampled Y-axis accel data
 * @param data_z Sampled Z-axis accel data
 * @return int8_t 
 */
int8_t server_send_adxl_data(system_defs *sys, int16_t *data_x, int16_t *data_y, int16_t *data_z, int reads)
{
    /** TODO: Add check at the end for OK message from tcp server */
    int err = 0;
    const char *load[3] = {"\x02\x01", "\x02\x02", "\x02\x03"};
    int16_t *datap[3];
    char rx_buffer[30];
    datap[0] = data_x;
    datap[1] = data_y;
    datap[2] = data_z;

    for (int i = 0; i < 3; i++)
    {
        //sprintf(hex_index, "%x", index_number);
        err = send(sys->socket, load[i], sizeof(load[i]), 0);
        if (err < 0)
        {
            ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
            return -1;
        }
        /** Send data to the TCP server. */
        err = send(sys->socket, datap[i], sizeof(int16_t) * reads, 0);
        if (err < 0)
        {
            ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
            return -1;
        }
        err = send(sys->socket, EndOfTrans, sizeof(EndOfTrans), 0);
        if (err < 0)
        {
            ESP_LOGE(TAG_SERVER, "Error occurred during sending: errno %d", errno);
            return -1;
        }
        /* Wait for confirmation from the server */
        int len = recv(sys->socket, rx_buffer, sizeof(rx_buffer) - 1, 0);
        // Error occurred during receiving
        if (len < 0)
        {
            ESP_LOGE(TAG_SERVER, "recv failed: errno %d", errno);
            return -1;
        }
        rx_buffer[len] = 0;
        ESP_LOGI(TAG_SERVER, "Received from server: %s", rx_buffer);
    }

    return 0;
}