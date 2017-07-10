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

    TemplateMsg::Subscriber sub1(participant, "Topic1");

    for(;;){
        //do something..

        for (int i = 0; i < sub1.get_data_seq().length(); ++i) {
            if (sub1.get_info_seq()[i].valid_data) {
                float sensor_val = sub1.get_data_seq()[i].value;
                printf("Received: %f\n", sensor_val);
            }
        }

        NDDSUtility::sleep(send_period);
    }
    delete participant;
    sub1.kill();
    return 0;
}