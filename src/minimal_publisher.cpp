#include "Boilerplate.h"
#include "ndds/ndds_cpp.h"
#include "TemplateBoilerplate.h"
#include "SensorsBoilerplate.h"

#define DOMAIN_ID 0

int main(int argc, char *argv[])
{
    DDS_Duration_t send_period = DDS_Duration_t::from_millis(5000);
        
    DDSBoilerplate * participant;
    participant = new DDSBoilerplate(DOMAIN_ID);

    TemplateMsg::Publisher pub1(participant, "Topic1");
    pub1.instance->id = (char*)"1";

    static float counter = 0;
           //do something..


        
    for(;;){
        pub1.instance->value = counter;
        pub1.publish(); 
        counter++;
        NDDSUtility::sleep(send_period);
    }
    delete participant;
    pub1.kill();
    return 0;
}