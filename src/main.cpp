#include "ndds/ndds_cpp.h"
#include "Boilerplate.h"

#define DOMAIN_ID 0

int main(int argc, char *argv[])
{
    DDS_Duration_t send_period = {1,0};
    Boilerplate participant(DOMAIN_ID);

    Subscriber sub1(participant, "Example Sensors");

    Publisher pub1(participant, "Example Sensors1");
    pub1.instance->id = "1";

    Publisher pub2(participant, "Example Sensors2");
    pub2.instance->id = "2";
    // Subscriber sub2(participant, "Example Sensors4");
    static int counter = 0;
    for(;;){

        for (int i = 0; i < sub1.reader_listener->data_seq.length(); ++i) {
            if (sub1.reader_listener->info_seq[i].valid_data) {
                float sensor_val = sub1.reader_listener->data_seq[i].value;
                printf("Value: %f\n", sensor_val);
            }
        }
        pub1.instance->value = counter++;
        pub1.publish(); 

        pub2.instance->value = counter++;
        pub2.publish(); 
        NDDSUtility::sleep(send_period);
    }

    pub1.kill();
    pub2.kill();
    sub1.kill();
    return 0;
}