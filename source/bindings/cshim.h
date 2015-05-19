#ifndef BINDING_C_SHIM_H
#define BINDING_C_SHIM_H

#ifndef FOR_FFI
    #include "../core/core.h"
    using namespace aserver;
//#else
//    #include <stdint.h>
#endif // FOR_FFI

#define AS_TYPE(type, obj) reinterpret_cast<type>(obj)

#ifdef __cplusplus
extern "C" {
#endif


struct core;
typedef struct core core_t;


// Generator Config Structures ************************************************

struct generator_cfg {
    uint64_t flags;
};
typedef struct generator_cfg generator_cfg_t;

struct primitive_cfg {
    generator_cfg_t config;
    
    unsigned amplitude;
    unsigned short frequency;
    unsigned short squareFactor;
    int waveform;
};
typedef struct primitive_cfg primitive_cfg_t;

struct test_cfg {
    primitive_cfg_t primitive;

    unsigned transitionPeriod;
    float frequencyScaleFactor;
    double distance;
    double angleStep;
    double initialAngle;
};
typedef struct test_cfg test_cfg_t;

struct wave_cfg {
    generator_cfg_t config;

    char filename[256];
    float increment;
    float position;
};
typedef struct wave_cfg wave_cfg_t;

struct noise_cfg {
    generator_cfg_t config;

    unsigned amplitude;
    int distType;
};
typedef struct noise_cfg noise_cfg_t;


// Processor Config Structures ************************************************

struct processor_cfg;
typedef struct processor_cfg processor_cfg_t;
typedef struct source_cfg source_cfg_t;


// Output Config Structures ***************************************************

struct output_cfg {
    uint64_t flags;
};
typedef struct output_cfg output_cfg_t;

struct file_cfg {
    output_cfg_t config;

    char outputFilePath[256];
    bool normalise_audio;
};
typedef struct file_cfg file_cfg_t;


// Shim API *******************************************************************

core_t* new_core();
void set_period(core_t* core, int size);
int add_generator(core_t* core, int type);
void configure_generator(core_t* core, int gid, generator_cfg_t* cfg);
int add_source(core_t* core);
void configure_source(core_t* core, int sid, source_cfg_t* cfg);
void set_processor(core_t* core, int type);
void configure_processor(core_t* core, processor_cfg_t* cfg);
void set_output(core_t* core, int type);
void configure_output(core_t* core, output_cfg_t* cfg);
void render(core_t* core, int periods);
void stop_output(core_t* core);
int16_t *get_output(core_t* core, uint64_t *size);
void free_output(core_t* core, int16_t *mem);
void del_core(core_t* core);

#ifdef __cplusplus
}
#endif

#endif
