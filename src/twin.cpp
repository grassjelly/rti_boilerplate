#include "Boilerplate.h"
#include "ndds/ndds_cpp.h"
#include "TemplateBoilerplate.h"
#include "SensorsBoilerplate.h"

#define DOMAIN_ID 0

int main(int argc, char *argv[])
{
    DDS_Duration_t send_period = DDS_Duration_t::from_millis(1000);
        
    DDSBoilerplate * participant;
    participant = new DDSBoilerplate(DOMAIN_ID);

    TemplateMsg::Publisher pub1(participant, "Example Template3");
    pub1.instance->id = (char*)"3";

    TemplateMsg::Publisher pub2(participant, "Example Template4");
    pub2.instance->id = (char*)"4";

    TemplateMsg::Subscriber sub1(participant, "Example Template1");

    TemplateMsg::Subscriber sub2(participant, "Example Template2");

    TestMsg::Subscriber sub3(participant, "Example Template5");

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

        for (int i = 0; i < sub3.get_data_seq().length(); ++i) {
            if (sub3.get_info_seq()[i].valid_data) {
                float sensor_val = sub3.get_data_seq()[i].value;
                printf("Value5: %f\n", sensor_val);
            }
        }

        pub1.instance->value = 3;
        pub1.publish(); 

        pub2.instance->value = 4;
        pub2.publish(); 
        NDDSUtility::sleep(send_period);
    }
    delete participant;
    pub1.kill();
    pub2.kill();
    sub1.kill();
    sub2.kill();
    return 0;
}