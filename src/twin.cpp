#include "ndds/ndds_cpp.h"
#include "Boilerplate.h"

#define DOMAIN_ID 0

int main(int argc, char *argv[])
{
    DDS_Duration_t send_period = {1,0};
        
    Boilerplate * participant;
    participant = new Boilerplate(DOMAIN_ID);

    Publisher pub1(participant, "Example Sensors3");
    pub1.instance->id = "3";

    Publisher pub2(participant, "Example Sensors4");
    pub2.instance->id = "4";

    Subscriber sub1(participant, "Example Sensors1");

    Subscriber sub2(participant, "Example Sensors2");

    static int counter = 0;
    for(;;){

        for (int i = 0; i < sub1.get_data_seq().length(); ++i) {
            if (sub1.get_info_seq()[i].valid_data) {
                float sensor_val = sub1.get_data_seq()[i].value;
                printf("Value1: %f\n", sensor_val);
            }
        }

        for (int i = 0; i < sub2.get_data_seq().length(); ++i) {
            if (sub2.get_info_seq()[i].valid_data) {
                float sensor_val = sub2.get_data_seq()[i].value;
                printf("Value2: %f\n", sensor_val);
            }
        }

        pub1.instance->value = 3;
        pub1.publish(); 

        pub2.instance->value = 4;
        pub2.publish(); 
        NDDSUtility::sleep(send_period);
    }

    pub1.kill();
    pub2.kill();
    sub1.kill();
    sub2.kill();
    return 0;
}