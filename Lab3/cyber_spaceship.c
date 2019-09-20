#include <stdio.h>

#include "cyber_spaceship.h"


int round_float_to_int(double number)
{
    if (number - (int)number >= 0.5) {
        return (int)number + 1;
    } else {
        return (int)number;
    }
}

const char* get_longest_safe_zone_or_null(const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count, size_t* out_longest_length)
{
    size_t i;
    const char* char_address;

    size_t zone_cluster_count;
    const char* longest_safe_cluster_start = NULL;
    size_t longest_cluster_length = 0;
    size_t current_safe_zone_in_row = 0;

    const char* cluster_start = NULL;
    const char* cluster_end = NULL;

    /* get cluster start and end */
    for (i = 0; i < cluster_count; i++) {
        if (i == 0) {
            cluster_start = cluster_start_locations[i];
            cluster_end = cluster_start_locations[i] + cluster_lengths[i] - 1;
        } else {
            if (cluster_start_locations[i] < cluster_start) {
                cluster_start = cluster_start_locations[i];
            }
            if (cluster_start_locations[i] + cluster_lengths[i] - 1 > cluster_end) {
                cluster_end = cluster_start_locations[i] + cluster_lengths[i] - 1;
            }
        }
    }

    /* find longest length and its start point */
    for (char_address = cluster_start; char_address <= cluster_end; char_address++) {
        zone_cluster_count = 0;
        for (i = 0; i < cluster_count; ++i) {
            if (char_address >= cluster_start_locations[i] && char_address < cluster_start_locations[i] + cluster_lengths[i]) {
                zone_cluster_count++;
            }
        }

        /* case dangerous */
        if (zone_cluster_count % 2 == 0) {
            if (current_safe_zone_in_row >= longest_cluster_length && current_safe_zone_in_row != 0) {
                longest_cluster_length = current_safe_zone_in_row;
                longest_safe_cluster_start = char_address - current_safe_zone_in_row;
            }
            current_safe_zone_in_row = 0;
        } else {
            current_safe_zone_in_row++;
        }
    }

    /* case all clusters are safe */
    if (longest_cluster_length == 0 && current_safe_zone_in_row != 0) {
        longest_cluster_length = current_safe_zone_in_row;
        longest_safe_cluster_start = cluster_start;
    }
    *out_longest_length = longest_cluster_length;
    return longest_safe_cluster_start;
}

int get_travel_time(const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count)
{
    size_t i;
    const char* char_address;

    size_t zone_cluster_count;

    const char* cluster_start = NULL;
    const char* cluster_end = NULL;

    int dangerous_count = 0;
    int safe_count = 0;
    int travel_time;

    /* get cluster start and end */
    for (i = 0; i < cluster_count; i++) {
        if (i == 0) {
            cluster_start = cluster_start_locations[i];
            cluster_end = cluster_start_locations[i] + cluster_lengths[i] - 1;
        } else {
            if (cluster_start_locations[i] < cluster_start) {
                cluster_start = cluster_start_locations[i];
            }
            if (cluster_start_locations[i] + cluster_lengths[i] - 1 > cluster_end) {
                cluster_end = cluster_start_locations[i] + cluster_lengths[i] - 1;
            }
        }
    }

    for (char_address = cluster_start; char_address <= cluster_end; char_address++) {
        zone_cluster_count = 0;
        for (i = 0; i < cluster_count; ++i) {
            if (char_address >= cluster_start_locations[i] && char_address < cluster_start_locations[i] + cluster_lengths[i]) {
                zone_cluster_count++;
            }
        }

        if (zone_cluster_count % 2 == 0) {
            dangerous_count++;
        } else {
            safe_count++;
        }
    }

    if (dangerous_count == 0) {
        travel_time = round_float_to_int((double)safe_count / TRAVEL_DISTANCE_PER_MIN_FOR_SAFE_ZONE);
    } else if (safe_count == 0) {
        travel_time = round_float_to_int((double)dangerous_count / TRAVEL_DISTANCE_PER_MIN_FOR_DANGEROUS_ZONE);
    } else {
        travel_time = round_float_to_int((double)dangerous_count / TRAVEL_DISTANCE_PER_MIN_FOR_DANGEROUS_ZONE + (double)safe_count / TRAVEL_DISTANCE_PER_MIN_FOR_SAFE_ZONE);
    }
    return travel_time;
}

