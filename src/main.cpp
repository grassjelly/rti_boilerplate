#include "ndds/ndds_cpp.h"
#include "Boilerplate.h"
#include "TemplateBoilerplate.h"
#include "SensorsBoilerplate.h"


#define DOMAIN_ID 0

int main(int argc, char *argv[])
{
    DDS_Duration_t send_period = {0,100000000};
        
    DDSBoilerplate * participant;
    participant = new DDSBoilerplate(DOMAIN_ID);

    TemplateMsg::Publisher pub1(participant, "Example Template1");
    pub1.instance->id = "1";

    TemplateMsg::Publisher pub2(participant, "Example Template2");
    pub2.instance->id = "2";

    TemplateMsg::Subscriber sub1(participant, "Example Template3");

    TemplateMsg::Subscriber sub2(participant, "Example Template4");

    TestMsg::Publisher pub3(participant, "Example Template5");

    static int counter = 0;
    for(;;){

        for (int i = 0; i < sub1.get_data_seq().length(); ++i) {
            if (sub1.get_info_seq()[i].valid_data) {
                float sensor_val = sub1.get_data_seq()[i].value;
                printf("Value3: %f\n", sensor_val);
            }
        }

        for (int i = 0; i < sub2.get_data_seq().length(); ++i) {
            if (sub2.get_info_seq()[i].valid_data) {
                float sensor_val = sub2.get_data_seq()[i].value;
                printf("Value4: %f\n", sensor_val);
            }
        }

        pub1.instance->value = 1;
        pub1.publish(); 

        pub2.instance->value = 2;
        pub2.publish(); 

        pub3.instance->value = 5;
        pub3.publish(); 
        NDDSUtility::sleep(send_period);
    }
    delete participant;
    pub1.kill();
    pub2.kill();
    pub3.kill();

    sub1.kill();
    sub2.kill();
    return 0;
}