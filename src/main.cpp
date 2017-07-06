#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "ndds/ndds_cpp.h"
#include "Boilerplate.h"

#define DOMAIN_ID 0

int main(int argc, char *argv[])
{
    DDS_Duration_t send_period = {1,0};
    Boilerplate participant(DOMAIN_ID);
    Publisher pub1(participant, "Example Sensors1");
    Publisher pub2(participant, "Example Sensors2");

    // SensorsListener *reader1;
    // reader1 = new SensorsListener();
    Subscriber sub1(participant, "Example Sensors");
    // SensorsListener *reader2;
    // reader2 = new SensorsListener();
    // Subscriber sub2(participant, "Example Sensors4", reader2);
    static int counter = 0;
    while(1){
        // int sub1_index = sub1.reader_listener->index;
        // float sensor_val = sub1.reader_listener->data_seq[sub1_index].value;
        // printf("Value: %f\n", sensor_val);

        for (int i = 0; i < sub1.reader_listener->data_seq.length(); ++i) {
            if (sub1.reader_listener->info_seq[i].valid_data) {
                float sensor_val = sub1.reader_listener->data_seq[i].value;
                printf("Value: %f\n", sensor_val);
            }
        }

        pub1.instance->value = 1;
        pub1.publish(); 

        pub2.instance->value = 2;
        pub2.publish(); 
        NDDSUtility::sleep(send_period);
    }
    return 0;
}