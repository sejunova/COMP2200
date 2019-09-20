#include <assert.h>
#include <stdio.h>
#include "cyber_spaceship.h"

int main() {
/* 아래의 코드는 다음과 같이 CAB를 설정함. -는 1바이트를 의미
         0                            29
CAB      ------------------------------
Cluster1             ---------------
Cluster2    -------------------------
*/
    const char cyber_asteroid_belt[30];
    const char* cluster_start_addresses[2];
    size_t cluster_lengths[2];
    size_t out_longest_length = 0;
    size_t* out_longest_length_p = &out_longest_length;
    const char* longest_safe_cluster_start_address;

    const char* cluster1 = &cyber_asteroid_belt[2];
    const char* cluster2 = &cyber_asteroid_belt[4];

    cluster_start_addresses[0] = cluster1;
    cluster_start_addresses[1] = cluster2;

    cluster_lengths[0] = 2U;
    cluster_lengths[1] = 2U;

    longest_safe_cluster_start_address = get_longest_safe_zone_or_null(cluster_start_addresses, cluster_lengths, 2, out_longest_length_p); /* longest_safe_cluster_start_address is (cyber_asteroid_belt + 3), out_longest_length: 9 */

    return 0;
}

