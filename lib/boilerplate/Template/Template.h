

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from Template.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Connext distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Connext manual.
*/

#ifndef Template_812788977_h
#define Template_812788977_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#else
#include "ndds_standalone_type.h"
#endif

extern "C" {

    extern const char *TemplateTYPENAME;

}

struct TemplateSeq;
#ifndef NDDS_STANDALONE_TYPE
class TemplateTypeSupport;
class TemplateDataWriter;
class TemplateDataReader;
#endif

class Template 
{
  public:
    typedef struct TemplateSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef TemplateTypeSupport TypeSupport;
    typedef TemplateDataWriter DataWriter;
    typedef TemplateDataReader DataReader;
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

NDDSUSERDllExport DDS_TypeCode* Template_get_typecode(void); /* Type code */

DDS_SEQUENCE(TemplateSeq, Template);                                        

NDDSUSERDllExport
RTIBool Template_initialize(
    Template* self);

NDDSUSERDllExport
RTIBool Template_initialize_ex(
    Template* self,RTIBool allocatePointers,RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool Template_initialize_w_params(
    Template* self,
    const struct DDS_TypeAllocationParams_t * allocParams);        

NDDSUSERDllExport
void Template_finalize(
    Template* self);

NDDSUSERDllExport
void Template_finalize_ex(
    Template* self,RTIBool deletePointers);

NDDSUSERDllExport
void Template_finalize_w_params(
    Template* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void Template_finalize_optional_members(
    Template* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool Template_copy(
    Template* dst,
    const Template* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* Template */

