#include "cshim.h"

extern "C" {

core_t* new_core()
{
    auto retv = new Core;
    //printf("new_core: %p\n", retv);
    return AS_TYPE(core_t*, retv);
}

// TODO: set_period_size

int add_generator(core_t* core, int type)
{
    return AS_TYPE(Core*, core)->addGenerator(static_cast<generator::types>(type));
}

/*
int configure_generator(core_t* core, int gid, generator_config_data_t* cfg)
{
    AS_TYPE(Core*, core)->generatorConfig(gid, AS_TYPE(generator::ConfigData*, cfg));
}
*/

int add_source(core_t* core)
{
    return AS_TYPE(Core*, core)->addSource();
}

/*
int configure_source(core_t* core, int sid, source_config_data_t* cfg)
{
    AS_TYPE(Core*, core)->sourceConfig(sid, AS_TYPE(processor::SourceConfigData*, cfg));
}
*/

void set_processor(core_t* core, int type)
{
    AS_TYPE(Core*, core)->setProcessor(static_cast<processor::types>(type));
}

/*
int configure_processor(core_t* core, processor_config_data_t* cfg)
{
    AS_TYPE(Core*, core)->processorConfig(AS_TYPE(processor::ConfigData*, cfg));
}
*/


void set_output(core_t* core, int type)
{
    AS_TYPE(Core*, core)->setOutput(static_cast<output::types>(type));
}

/*
int configure_output(core_t* core, output_config_data_t* cfg)
{
    AS_TYPE(Core*, core)->outputConfig(AS_TYPE(output::ConfigData*, cfg));
}
*/

void render(core_t* core, int periods)
{
    for (int i = 0; i < periods; i++) {
        AS_TYPE(Core*, core)->render();
    }
}


void del_core(core_t* core)
{
    //printf("del_core: %p\n", core);
    AS_TYPE(Core*, core)->shutdown();
    delete AS_TYPE(Core*, core);
}


} // extern "C" 

