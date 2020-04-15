/* Copyright 2020, Edwin Chiu

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdint.h>

#include "checksum.h"

uint16_t checksum_fletcher16(uint16_t *sum1, uint16_t *sum2,  char *data, uint8_t count)
{
    uint8_t index;

    for (index = 0; index < count; ++index)
    {
        *sum1 = ((*sum1) + data[index]) % 255;
        *sum2 = ((*sum2) + (*sum1)) % 255;
    }

    return ((*sum2) << 8) | *sum1;
}

bool checksum_check(char *packet, uint8_t packet_len) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    uint16_t csum;
    uint8_t c0,c1,f0,f1;

    // TODO Check validity of packet_len ?
    // TODO Can this code be optimised?

    csum = checksum_fletcher16(&sum1, &sum2, packet, packet_len-2); // Don't check the final two check bytes
    f0 = csum & 0xff;
    f1 = (csum >> 8) & 0xff;
    c0 = 0xff - ((f0 + f1) % 0xff);
    c1 = 0xff - ((f0 + c0) % 0xff);

    if((packet[packet_len-2] == (char) c0) && (packet[packet_len-1] == (char) c1))
        return true;

    return false;
}
