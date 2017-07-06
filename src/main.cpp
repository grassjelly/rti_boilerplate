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

    SensorsListener *reader1;
    reader1 = new SensorsListener();
    Subscriber sub1(participant, "Example Sensors3", reader1);

    SensorsListener *reader2;
    reader2 = new SensorsListener();
    Subscriber sub2(participant, "Example Sensors4", reader2);

    static int counter = 0;
    while(1){
        std::cout << "Sending.." << std::endl;
        pub1.instance->value = 1;
        pub1.publish(); 

        pub2.instance->value = 2;
        pub2.publish(); 
        NDDSUtility::sleep(send_period);
    }
    return 0;
}