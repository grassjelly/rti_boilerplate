#include "Boilerplate.h"
#include "TemplateBoilerplate.h"
#include "TemplateSupport.h"
#include <string> //ADDED

TemplateMsg::Publisher::Publisher(DDSBoilerplate * boiler_object, char const * user_topic)
{
    user_topic_ = user_topic;
    boiler_object_ = boiler_object;
    participant_ = boiler_object_->get_participant_obj();
    init_publisher();
}

int TemplateMsg::Publisher::init_publisher()
{
    DDSPublisher *publisher;
    DDSTopic *topic = NULL;
    DDSDataWriter *writer = NULL;
    Template_writer_ = NULL;
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
    type_name = TemplateTypeSupport::get_type_name();
    retcode_ = TemplateTypeSupport::register_type(
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

    Template_writer_ = TemplateDataWriter::narrow(writer);
    if (Template_writer_ == NULL) {
        printf("DataWriter narrow error\n");
        boiler_object_->node_shutdown();
        return -1;
    }

    /* Create data sample for writing */
    instance = TemplateTypeSupport::create_data();
    if (instance == NULL) {
        printf("TemplateTypeSupport::create_data error\n");
        boiler_object_->node_shutdown();
        return -1;
    }
}

void TemplateMsg::Publisher::publish(){
    retcode_ = Template_writer_->write(*instance, instance_handle_);

    if (retcode_ != DDS_RETCODE_OK) {
        printf("write error %d\n", retcode_);
    }
}

int TemplateMsg::Publisher::kill()
{
    retcode_ = TemplateTypeSupport::delete_data(instance);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("TemplateTypeSupport::delete_data error %d\n", retcode_);
    }

    /* Delete all entities */
    return boiler_object_->node_shutdown();
}

TemplateMsg::Subscriber::Subscriber(DDSBoilerplate * boiler_object, char const * user_topic)
{
    user_topic_ = user_topic;
    boiler_object_ = boiler_object;
    participant_ = boiler_object_->get_participant_obj();
    init_subscriber();
}

void TemplateMsg::TemplateListener::on_data_available(DDSDataReader* reader) 
{
    TemplateDataReader *Template_reader = NULL;
    TemplateSeq data_seq_;
    DDS_SampleInfoSeq info_seq_;    
    DDS_ReturnCode_t retcode;
    int i;

    Template_reader = TemplateDataReader::narrow(reader);
    if (Template_reader == NULL) {
        printf("DataReader narrow error\n");
        return;
    }

    retcode = Template_reader->take(
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
    //         //TemplateTypeSupport::print_data(&data_seq[i]);
    //     }
    // }

    retcode = Template_reader->return_loan(data_seq_, info_seq_);
    if (retcode != DDS_RETCODE_OK) {
        printf("return loan error %d\n", retcode);
    }
}

int TemplateMsg::Subscriber::init_subscriber()
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
    type_name = TemplateTypeSupport::get_type_name();
    retcode_ = TemplateTypeSupport::register_type(
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
    reader_listener_ = new TemplateListener();
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

TemplateSeq TemplateMsg::Subscriber::get_data_seq()
{
    return reader_listener_->data_seq;
}

DDS_SampleInfoSeq TemplateMsg::Subscriber::get_info_seq()
{
    return reader_listener_->info_seq;
}

bool TemplateMsg::Subscriber::is_read()
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

int TemplateMsg::Subscriber::kill()
{
    int status = boiler_object_->node_shutdown();
    delete reader_listener_;

    return status;
}

TemplateMsg::Subscriber::Subscriber(DDSBoilerplate * boiler_object, char const * user_topic, char const * attribute) //ADDED
{
    user_topic_ = user_topic;
    boiler_object_ = boiler_object;
    participant_ = boiler_object_->get_participant_obj();
    init_subscriber(attribute);
}

int TemplateMsg::Subscriber::init_subscriber(char const * attribute) //ADDED
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
    type_name = TemplateTypeSupport::get_type_name();
    retcode_ = TemplateTypeSupport::register_type(
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
    reader_listener_ = new TemplateListener();
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

void TemplateMsg::Subscriber::set_string_filter(char* filter) //ADDED
{
    retcode_ = cft_->append_to_expression_parameter(0, filter);
    if (retcode_ != DDS_RETCODE_OK) {
        printf("append_to_expression_parameter error\n");
        boiler_object_->node_shutdown();
    }
}

