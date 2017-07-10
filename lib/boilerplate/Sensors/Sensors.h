

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Sensors.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef Sensors_1082808643_h
#define Sensors_1082808643_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#else
#include "ndds_standalone_type.h"
#endif

extern "C" {

    extern const char *SensorsTYPENAME;

}

struct SensorsSeq;
#ifndef NDDS_STANDALONE_TYPE
class SensorsTypeSupport;
class SensorsDataWriter;
class SensorsDataReader;
#endif

class Sensors 
{
  public:
    typedef struct SensorsSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef SensorsTypeSupport TypeSupport;
    typedef SensorsDataWriter DataWriter;
    typedef SensorsDataReader DataReader;
    #endif

    DDS_Char *   id ;
    DDS_Char *   type ;
    DDS_Float   value ;

};
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

NDDSUSERDllExport DDS_TypeCode* Sensors_get_typecode(void); /* Type code */

DDS_SEQUENCE(SensorsSeq, Sensors);                                        

NDDSUSERDllExport
RTIBool Sensors_initialize(
    Sensors* self);

NDDSUSERDllExport
RTIBool Sensors_initialize_ex(
    Sensors* self,RTIBool allocatePointers,RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool Sensors_initialize_w_params(
    Sensors* self,
    const struct DDS_TypeAllocationParams_t * allocParams);        

NDDSUSERDllExport
void Sensors_finalize(
    Sensors* self);

NDDSUSERDllExport
void Sensors_finalize_ex(
    Sensors* self,RTIBool deletePointers);

NDDSUSERDllExport
void Sensors_finalize_w_params(
    Sensors* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void Sensors_finalize_optional_members(
    Sensors* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool Sensors_copy(
    Sensors* dst,
    const Sensors* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* Sensors */

