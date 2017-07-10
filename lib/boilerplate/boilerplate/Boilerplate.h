#ifndef BOILERPLATE_H
#define BOILERPLATE_H
#include "ndds/ndds_cpp.h"

class DDSBoilerplate
{
    public:
        DDSBoilerplate(int domainId);
        int node_shutdown();
        DDSDomainParticipant * get_participant_obj();
        DDS_ReturnCode_t get_retcode();
        
        int x();

    private:
        int init_domain_participant(int domainId);
        DDSDomainParticipant *participant_;
        DDS_ReturnCode_t retcode_;
};
#endif