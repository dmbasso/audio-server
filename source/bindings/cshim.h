#ifndef BINDING_C_SHIM_H
#define BINDING_C_SHIM_H

#ifndef FOR_FFI
    #include "../core/core.h"
    using namespace aserver;
    #pragma pack(1)
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
    float amplitude;
    uint16_t frequency;
    uint16_t squareFactor;
    int32_t waveform;
};
typedef struct primitive_cfg primitive_cfg_t;

struct test_cfg {
    primitive_cfg_t primitive;

    uint32_t transitionPeriod;
    float frequencyScaleFactor;
    double distance;
    double angleStep;
    double initialAngle;
};
typedef struct test_cfg test_cfg_t;

struct wave_cfg {
    generator_cfg_t config;

    uint8_t command;
    int16_t waveIndex;
    float frequencyRatio;
    float wavePosition;
};
typedef struct wave_cfg wave_cfg_t;

struct noise_cfg {
    generator_cfg_t config;

    uint32_t amplitude;
    int32_t distType;
};
typedef struct noise_cfg noise_cfg_t;

struct keyframe_s {
    wave_cfg_t wave;

    uint64_t start;
    float location[3];
};
typedef struct keyframe_s keyframe_t;

struct script_cfg {
    generator_cfg_t config;

    uint8_t command;
    uint32_t keyframeCount;
    keyframe_t *keyframes;
};
typedef struct script_cfg script_cfg_t;


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
void set_period(core_t* core, int32_t size);
int16_t get_wave_index(core_t* core, const char* filename);
int16_t add_wave(core_t* core, uint32_t size, uint8_t channels, const int16_t *samples);
int16_t add_generator(core_t* core, int32_t type);
void configure_generator(core_t* core, uint16_t gid, generator_cfg_t* cfg);
uint16_t add_source(core_t* core);
void configure_source(core_t* core, uint16_t sid, source_cfg_t* cfg);
void set_source_generator(core_t* core, uint16_t sid, uint16_t gid);
void set_processor(core_t* core, int32_t type);
void configure_processor(core_t* core, processor_cfg_t* cfg);
void set_output(core_t* core, int32_t type);
void configure_output(core_t* core, output_cfg_t* cfg);
void render(core_t* core, int32_t periods);
void stop_output(core_t* core);
int16_t *get_output(core_t* core, uint64_t *size);
void free_output(core_t* core, int16_t *mem);
void del_core(core_t* core);
void reset(core_t* core, bool reset_output);
#ifdef __cplusplus
}
#endif

#ifndef FOR_FFI
    #pragma pack()
#endif

#endif
