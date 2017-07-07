#ifndef BOILERPLATE_H
#define BOILERPLATE_H
#include "Sensors.h"
#include "SensorsSupport.h"
#include "ndds/ndds_cpp.h"

class Boilerplate
{
    public:
        Boilerplate(int domainId);
        int node_shutdown();
        DDSDomainParticipant * get_participant_obj();
        DDS_ReturnCode_t get_retcode();
        
        int x();

    private:
        int init_domain_participant(int domainId);
        DDSDomainParticipant *participant_;
        DDS_ReturnCode_t retcode_;
};

class Publisher
{
    public:
        Publisher(Boilerplate& participant_object, char const * user_topic );
        Sensors *instance;
        int init_publisher();
        void publish();
        int kill();

    private:
        char const * user_topic_;
        Boilerplate& boiler_object_;
        DDSDomainParticipant *participant_;
        SensorsDataWriter * Sensors_writer_;
        DDS_ReturnCode_t retcode_;
        DDS_InstanceHandle_t instance_handle_ ;
};


class SensorsListener : public DDSDataReaderListener 
{
    public:
        virtual void on_requested_deadline_missed(
            DDSDataReader* /*reader*/,
            const DDS_RequestedDeadlineMissedStatus& /*status*/) {}

        virtual void on_requested_incompatible_qos(
            DDSDataReader* /*reader*/,
            const DDS_RequestedIncompatibleQosStatus& /*status*/) {}

        virtual void on_sample_rejected(
            DDSDataReader* /*reader*/,
            const DDS_SampleRejectedStatus& /*status*/) {}

        virtual void on_liveliness_changed(
            DDSDataReader* /*reader*/,
            const DDS_LivelinessChangedStatus& /*status*/) {}

        virtual void on_sample_lost(
            DDSDataReader* /*reader*/,
            const DDS_SampleLostStatus& /*status*/) {}

        virtual void on_subscription_matched(
            DDSDataReader* /*reader*/,
            const DDS_SubscriptionMatchedStatus& /*status*/) {}

        virtual void on_data_available(DDSDataReader* reader);

        SensorsSeq data_seq;
        DDS_SampleInfoSeq info_seq;
};

class Subscriber
{
    public:
        Subscriber(Boilerplate& participant_object, char const * user_topic);
        int init_subscriber();
        int kill();
        SensorsSeq get_data_seq();
        DDS_SampleInfoSeq get_info_seq();

    private:
        char const * user_topic_;
        Boilerplate& boiler_object_;
        DDSDomainParticipant *participant_;
        SensorsListener *reader_listener_;
        DDS_ReturnCode_t retcode_;
};

#endif