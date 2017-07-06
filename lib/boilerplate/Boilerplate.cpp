#include "Boilerplate.h"
#include "Sensors.h"
#include "SensorsSupport.h"
#include "ndds/ndds_cpp.h"

Boilerplate::Boilerplate(int domainId)
{
  init_domain_participant(domainId);
}

int Boilerplate::node_shutdown()
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

int Boilerplate::init_domain_participant(int domainId){
  DDSDomainParticipant *participant = NULL;
  participant_ = participant;

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

Publisher::Publisher(Boilerplate& participant_object, char const * user_topic) : boiler_participant_(participant_object)
{
    user_topic_ = user_topic;
    init_publisher();
}

int Publisher::init_publisher()
{
    DDSPublisher *publisher;
    DDSTopic *topic = NULL;
    DDSDataWriter *writer = NULL;
    Sensors_writer_ = NULL;
    instance = NULL;
    instance_handle_ = DDS_HANDLE_NIL;
    const char *type_name = NULL;

    /* To customize publisher QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    publisher = boiler_participant_.participant_->create_publisher(
        DDS_PUBLISHER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (publisher == NULL) {
        printf("create_publisher error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }
    
    /* Register type before creating topic */
    type_name = SensorsTypeSupport::get_type_name();
    boiler_participant_.retcode_ = SensorsTypeSupport::register_type(
        boiler_participant_.participant_, type_name);
    if (boiler_participant_.retcode_ != DDS_RETCODE_OK) {
        printf("register_type error %d\n", boiler_participant_.retcode_);
        boiler_participant_.node_shutdown();
        return -1;
    }

/* To customize topic QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    topic = boiler_participant_.participant_->create_topic(
        user_topic_,
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);    // y.publish();

    if (topic == NULL) {
        printf("create_topic error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }
    
    /* To customize data writer QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    writer = publisher->create_datawriter(
        topic, DDS_DATAWRITER_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if (writer == NULL) {
        printf("create_datawriter error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }

    Sensors_writer_ = SensorsDataWriter::narrow(writer);
    if (Sensors_writer_ == NULL) {
        printf("DataWriter narrow error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }

    /* Create data sample for writing */
    instance = SensorsTypeSupport::create_data();
    if (instance == NULL) {
        printf("SensorsTypeSupport::create_data error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }
}

void Publisher::publish(){
    boiler_participant_.retcode_ = Sensors_writer_->write(*instance, instance_handle_);

    if (boiler_participant_.retcode_ != DDS_RETCODE_OK) {
        printf("write error %d\n", boiler_participant_.retcode_);
    }
}

int Publisher::kill()
{
    boiler_participant_.retcode_ = SensorsTypeSupport::delete_data(instance);
    if (boiler_participant_.retcode_ != DDS_RETCODE_OK) {
        printf("SensorsTypeSupport::delete_data error %d\n", boiler_participant_.retcode_);
    }

    /* Delete all entities */
    return boiler_participant_.node_shutdown();
}

Subscriber::Subscriber(Boilerplate& participant_object, char const * user_topic) : boiler_participant_(participant_object)
{
    user_topic_ = user_topic;
    init_subscriber();
}

void SensorsListener::on_data_available(DDSDataReader* reader) 
{
    SensorsDataReader *Sensors_reader = NULL;
    SensorsSeq data_seq_;
    DDS_SampleInfoSeq info_seq_;    
    DDS_ReturnCode_t retcode;
    int i;

    Sensors_reader = SensorsDataReader::narrow(reader);
    if (Sensors_reader == NULL) {
        printf("DataReader narrow error\n");
        return;
    }

    retcode = Sensors_reader->take(
        data_seq_, info_seq_, DDS_LENGTH_UNLIMITED,
        DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (retcode == DDS_RETCODE_NO_DATA) {
        return;
    } else if (retcode != DDS_RETCODE_OK) {
        printf("take error %d\n", retcode);
        return;
    }

    this->info_seq = info_seq_;
    this->data_seq = data_seq_;

    // for (i = 0; i < data_seq_.length(); ++i) {
    //     if (info_seq_[i].valid_data) {
    //         std::cout << info_seq_[i].valid_data << std::endl;
    //         //printf("Received data\n");
    //         //SensorsTypeSupport::print_data(&data_seq[i]);
    //     }
    // }

    retcode = Sensors_reader->return_loan(data_seq_, info_seq_);
    if (retcode != DDS_RETCODE_OK) {
        printf("return loan error %d\n", retcode);
    }
}
int Subscriber::init_subscriber()
{
    DDSSubscriber *subscriber;
    DDSTopic *topic = NULL;
    DDSDataReader *reader = NULL;
    const char *type_name = NULL;

    /* To customize publisher QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    subscriber = boiler_participant_.participant_->create_subscriber(
        DDS_SUBSCRIBER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }
    
    /* Register type before creating topic */
    type_name = SensorsTypeSupport::get_type_name();
    boiler_participant_.retcode_ = SensorsTypeSupport::register_type(
        boiler_participant_.participant_, type_name);
    if (boiler_participant_.retcode_ != DDS_RETCODE_OK) {
        printf("register_type error %d\n", boiler_participant_.retcode_);
        boiler_participant_.node_shutdown();
        return -1;
    }

/* To customize topic QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    topic = boiler_participant_.participant_->create_topic(
        user_topic_,
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);    
    if (topic == NULL) {
        printf("create_topic error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }

    /* Create a data reader listener */
    reader_listener = new SensorsListener();
        /* To customize data writer QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    reader = subscriber->create_datareader(
        topic, DDS_DATAREADER_QOS_DEFAULT, reader_listener /* listener */,
        DDS_STATUS_MASK_ALL);
    if (reader == NULL) {
        printf("create_datareader error\n");
        boiler_participant_.node_shutdown();
        delete reader_listener;
        return -1;
    }
}

int Subscriber::kill()
{
    int status = boiler_participant_.node_shutdown();
    delete reader_listener;

    return status;
}