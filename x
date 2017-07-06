Subscriber::Subscriber(Boilerplate& participant_object, char const * user_topic)  boiler_participant_(participant_object)
{
    user_topic_ = user_topic;
    init_subscriber();
}

int Subscriber::init_subscriber()
{
    DDSPublisher *publisher;

    DDSTopic *topic = NULL;
    DDSDataReader *reader = NULL;
    Sensors_writer_ = NULL;
    instance_ = NULL;
    instance_handle_ = DDS_HANDLE_NIL;
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
        DDS_STATUS_MASK_NONE);    // y.publish();

    if (topic == NULL) {
        printf("create_topic error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }
    
    /* To customize data writer QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    reader = subscriber->create_datawriter(
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
    instance_ = SensorsTypeSupport::create_data();
    if (instance_ == NULL) {
        printf("SensorsTypeSupport::create_data error\n");
        boiler_participant_.node_shutdown();
        return -1;
    }
}


