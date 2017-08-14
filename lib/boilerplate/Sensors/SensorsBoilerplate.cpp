#include "Boilerplate.h"
#include "SensorsBoilerplate.h"
#include "SensorsSupport.h"
#include <string> //ADDED

TestMsg::Publisher::Publisher(DDSBoilerplate * boiler_object, char const * user_topic)
{
    user_topic_ = user_topic;
    boiler_object_ = boiler_object;
    participant_ = boiler_object_->get_participant_obj();
    init_publisher();
}

int TestMsg::Publisher::init_publisher()
{
    DDSPublisher *publisher;
    DDSTopic *topic = NULL;
    DDSDataWriter *writer = NULL;
    Test_writer_ = NULL;
    instance = NULL;
    instance_handle_ = DDS_HANDLE_NIL;
    const char *type_name = NULL;
    
    /* To customize publisher QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    publisher = participant_->create_publisher(
        DDS_PUBLISHER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (publisher == NULL) {
        printf("create_publisher error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
    
    /* Register type before creating topic */
    type_name = TestTypeSupport::get_type_name();
    retcode_ = TestTypeSupport::register_type(
        participant_, type_name);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode_);
        boiler_object_->node_shutdown();
        return -1;
    }

/* To customize topic QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    topic = participant_->create_topic(
        user_topic_,
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);    // y.publish();

    if (topic == NULL) {
        printf("create_topic error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
    
    /* To customize data writer QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    writer = publisher->create_datawriter(
        topic, DDS_DATAWRITER_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if (writer == NULL) {
        printf("create_datawriter error\n");
        boiler_object_->node_shutdown();
        return -1;
    }

    Test_writer_ = TestDataWriter::narrow(writer);
    if (Test_writer_ == NULL) {
        printf("DataWriter narrow error\n");
        boiler_object_->node_shutdown();
        return -1;
    }

    /* Create data sample for writing */
    instance = TestTypeSupport::create_data();
    if (instance == NULL) {
        printf("TestTypeSupport::create_data error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
}

void TestMsg::Publisher::publish(){
    retcode_ = Test_writer_->write(*instance, instance_handle_);

    if (retcode_ != DDS_RETCODE_OK) {
        printf("write error %d\n", retcode_);
    }
}

int TestMsg::Publisher::kill()
{
    retcode_ = TestTypeSupport::delete_data(instance);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("TestTypeSupport::delete_data error %d\n", retcode_);
    }

    /* Delete all entities */
    return boiler_object_->node_shutdown();
}

TestMsg::Subscriber::Subscriber(DDSBoilerplate * boiler_object, char const * user_topic)
{
    user_topic_ = user_topic;
    boiler_object_ = boiler_object;
    participant_ = boiler_object_->get_participant_obj();
    init_subscriber();
}

void TestMsg::TestListener::on_data_available(DDSDataReader* reader) 
{
    TestDataReader *Test_reader = NULL;
    TestSeq data_seq_;
    DDS_SampleInfoSeq info_seq_;    
    DDS_ReturnCode_t retcode;
    int i;

    Test_reader = TestDataReader::narrow(reader);
    if (Test_reader == NULL) {
        printf("DataReader narrow error\n");
        return;
    }

    retcode = Test_reader->take(
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
    //         //TestTypeSupport::print_data(&data_seq[i]);
    //     }
    // }

    retcode = Test_reader->return_loan(data_seq_, info_seq_);
    if (retcode != DDS_RETCODE_OK) {
        printf("return loan error %d\n", retcode);
    }
}

int TestMsg::Subscriber::init_subscriber()
{
    DDSSubscriber *subscriber;
    DDSTopic *topic = NULL;
    DDSDataReader *reader = NULL;
    const char *type_name = NULL;

    /* To customize publisher QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    subscriber = participant_->create_subscriber(
        DDS_SUBSCRIBER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
    
    /* Register type before creating topic */
    type_name = TestTypeSupport::get_type_name();
    retcode_ = TestTypeSupport::register_type(
        participant_, type_name);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode_);
        boiler_object_->node_shutdown();
        return -1;
    }

/* To customize topic QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    topic = participant_->create_topic(
        user_topic_,
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);    
    if (topic == NULL) {
        printf("create_topic error\n");
        boiler_object_->node_shutdown();
        return -1;
    }

    /* Create a data reader listener */
    reader_listener_ = new TestListener();
        /* To customize data writer QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    reader = subscriber->create_datareader(
        topic, DDS_DATAREADER_QOS_DEFAULT, reader_listener_ /* listener */,
        DDS_STATUS_MASK_ALL);
    if (reader == NULL) {
        printf("create_datareader error\n");
        boiler_object_->node_shutdown();
        delete reader_listener_;
        return -1;
    }
}

TestSeq TestMsg::Subscriber::get_data_seq()
{
    return reader_listener_->data_seq;
}

DDS_SampleInfoSeq TestMsg::Subscriber::get_info_seq()
{
    return reader_listener_->info_seq;
}

bool TestMsg::Subscriber::is_read()
{
    if(reader_listener_->info_seq.length()==0) //this condition fixes a bug with not receiving the first subscribed data.
        return true;
    for (int i = 0; i <reader_listener_->info_seq.length(); i++){
        if(reader_listener_->info_seq[i].sample_state == DDS_READ_SAMPLE_STATE){
            return true;
        }
        reader_listener_->info_seq[i].sample_state = DDS_READ_SAMPLE_STATE;
    }
    return false;
}

int TestMsg::Subscriber::kill()
{
    int status = boiler_object_->node_shutdown();
    delete reader_listener_;

    return status;
}

TestMsg::Subscriber::Subscriber(DDSBoilerplate * boiler_object, char const * user_topic, char const * attribute) //ADDED
{
    user_topic_ = user_topic;
    boiler_object_ = boiler_object;
    participant_ = boiler_object_->get_participant_obj();
    init_subscriber(attribute);
}

int TestMsg::Subscriber::init_subscriber(char const * attribute) //ADDED
{
    DDSSubscriber *subscriber;
    DDSTopic *topic = NULL;
    DDSDataReader *reader = NULL;
    const char *type_name = NULL;

    /* To customize publisher QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    subscriber = participant_->create_subscriber(
        DDS_SUBSCRIBER_QOS_DEFAULT, NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
    
    /* Register type before creating topic */
    type_name = TestTypeSupport::get_type_name();
    retcode_ = TestTypeSupport::register_type(
        participant_, type_name);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode_);
        boiler_object_->node_shutdown();
        return -1;
    }

/* To customize topic QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    topic = participant_->create_topic(
        user_topic_,
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);    
    if (topic == NULL) {
        printf("create_topic error\n");
        boiler_object_->node_shutdown();
        return -1;
    }

     DDS_StringSeq parameters(1);
    const char* param_list[] = {"HELLO"};
    parameters.from_array(param_list, 1);

    cft_ = NULL;

    cft_ = participant_->create_contentfilteredtopic_with_filter(
        user_topic_, topic, &((std::string(attribute)+std::string(" MATCH %0")).c_str())[0], parameters,
        DDS_STRINGMATCHFILTER_NAME);
    if (cft_ == NULL) {
        printf("create_contentfilteredtopic error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
    

    /* Create a data reader listener */
    reader_listener_ = new TestListener();
        /* To customize data writer QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    reader = subscriber->create_datareader(
        cft_, DDS_DATAREADER_QOS_DEFAULT, reader_listener_ /* listener */,
        DDS_STATUS_MASK_ALL);
    if (reader == NULL) {
        printf("create_datareader error\n");
        boiler_object_->node_shutdown();
        delete reader_listener_;
        return -1;
    }
}

void TestMsg::Subscriber::set_string_filter(char* filter) //ADDED
{
    retcode_ = cft_->append_to_expression_parameter(0, filter);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("append_to_expression_parameter error\n");
        boiler_object_->node_shutdown();
    }
}

