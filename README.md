# rti_boilerplate
This package allows a user to create multiple instances of publisher and subscriber within one node. The API supports instances of different message types defined in an IDL file.

## Adding new IDL file:

1. Add your own idl file in the idl folder.
2. Generate the codes:
    
    $ ./gencode

This script will auto:  
- Generate the DDS codes for the idl files you have created. 
- Generate boilerplates specific to the message type so you can use the API.
- Generate a CMakeLists.txt for the boilerplates. This must be included in the main CMakeLists.txt so you can use it in the API. See No.3 .

3. Add the generated codes and boilerplates in the main CMakeLists.txt:

        include(lib/boilerplate/Template/CMakeLists.txt)
        include(lib/boilerplate/Sensors/CMakeLists.txt)
        include(lib/boilerplate/<IDL_NAME>/CMakeLists.txt)

        add_executable(main
            src/main.cpp
            ${BOILERPLATE_SOURCE}
            ${SOURCE_Template}
            ${SOURCE_Sensors}
            ${SOURCE_<IDLNAME>}
        )

## Using the API:

1. Include your new message type's header files:
    
        #include "TemplateBoilerplate.h"

2. Create participant:
    
        DDSBoilerplate * participant;
        participant = new DDSBoilerplate(DOMAIN_ID);

3. Creating a Publisher:
  
        TemplateMsg::Publisher pub1(participant, "Example Template1");

TemplateMsg can be any message based on the idl file you have created.

4. Publish:

        pub1.instance->value = 1;
        pub1.publish(); 

5. Creating a Subscriber:

        TemplateMsg::Subscriber sub1(participant, "Example Template3");


TemplateMsg can be any message based on the idl file you have created.

6. Subscribing and receiving data:

       for (int i = 0; i < sub1.get_data_seq().length(); ++i) {
            if (sub1.get_info_seq()[i].valid_data) {
                float sensor_val = sub1.get_data_seq()[i].value;
                printf("Value3: %f\n", sensor_val);
            }
        }