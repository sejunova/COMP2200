#include <assert.h>
#include <stdio.h>
#include "cyber_spaceship.h"

int main() {
    char cyber_asteroid_belt[50];
    const char* cluster_start_addresses[5];
    size_t cluster_lengths[5];
    size_t out_longest_length = 0;
    size_t* out_longest_length_p = &out_longest_length;
    const char* longest_safe_cluster_start_address;
    int time_in_mins;

    cluster_start_addresses[0] = &cyber_asteroid_belt[10];
    cluster_start_addresses[1] = &cyber_asteroid_belt[2];
    cluster_start_addresses[2] = &cyber_asteroid_belt[25];
    cluster_start_addresses[3] = &cyber_asteroid_belt[35];
    cluster_start_addresses[4] = &cyber_asteroid_belt[40];

    cluster_lengths[0] = 15U;
    cluster_lengths[1] = 12U;
    cluster_lengths[2] = 10U;
    cluster_lengths[3] = 10U;
    cluster_lengths[4] = 8U;

    longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cluster_start_addresses, cluster_lengths, 5, out_longest_length_p);

    assert(out_longest_length == 26);
    assert(longest_safe_cluster_start_address == cyber_asteroid_belt + 14);

    time_in_mins = get_travel_time(cluster_start_addresses, cluster_lengths, 5);

    assert(time_in_mins == 6);

    return 0;
}

