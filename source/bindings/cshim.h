#ifndef BINDING_C_SHIM_H
#define BINDING_C_SHIM_H

#ifndef FOR_FFI
    #include "../core/core.h"
    using namespace aserver;
#endif // FOR_FFI

#define AS_TYPE(type, obj) reinterpret_cast<type>(obj)

#ifdef __cplusplus
extern "C" {
#endif

struct core;
struct generator_config_data;
struct processor_config_data;
struct output_config_data;

typedef struct core core_t;
typedef struct generator_config_data generator_config_data_t;
typedef struct processor_config_data processor_config_data_t;
typedef struct source_config_data source_config_data_t;
typedef struct output_config_data output_config_data_t;

core_t* new_core();
// TODO: set_period_size
int add_generator(core_t* core, int type);
int configure_generator(core_t* core, int gid, generator_config_data_t* cfg);
int add_source(core_t* core);
int configure_source(core_t* core, int sid, source_config_data_t* cfg);
void set_processor(core_t* core, int type);
int configure_processor(core_t* core, processor_config_data_t* cfg);
void set_output(core_t* core, int type);
int configure_output(core_t* core, output_config_data_t* cfg);
void render(core_t* core, int periods);
void del_core(core_t* core);

#ifdef __cplusplus
}
#endif

#endif
