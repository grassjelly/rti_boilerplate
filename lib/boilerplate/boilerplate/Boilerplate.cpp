#include "Boilerplate.h"
#include "ndds/ndds_cpp.h"

DDSBoilerplate::DDSBoilerplate(int domainId)
{
  init_domain_participant(domainId);
}

int DDSBoilerplate::node_shutdown()
{
    int status = 0;

    if (participant_ != NULL) {
        retcode_ = participant_->delete_contained_entities();
        if (retcode_ != DDS_RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode_);
            status = -1;
        }

        retcode_ = DDSTheParticipantFactory->delete_participant(participant_);
        if (retcode_ != DDS_RETCODE_OK) {
            printf("delete_participant error %d\n", retcode_);
            status = -1;
        }
    }

    return status;
}

int DDSBoilerplate::init_domain_participant(int domainId)
{
    participant_ = NULL;

    DDS_InstanceHandle_t instance_handle = DDS_HANDLE_NIL;
    DDS_Duration_t send_period = {4,0};

    /* To customize participant QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    participant_ = DDSTheParticipantFactory->create_participant(
        domainId, DDS_PARTICIPANT_QOS_DEFAULT, 
        NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (participant_ == NULL) {
        printf("create_participant error\n");
        node_shutdown();
        return -1;
    }
}

DDSDomainParticipant * DDSBoilerplate::get_participant_obj()
{
    return participant_;
}
