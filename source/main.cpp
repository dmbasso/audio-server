/**
 * This is the main file for the audio-server.
 * It contains methods for running the audio-server main loop, etc.
 * The following figure ilustrates the overall architecture.
 * \image latex classes.png "audio-server architecture draft" width=14cm
 *
 */

#include "generator.h"
#include "core.h"
#include "output.h"

#include <iostream>

using namespace aserver;
using namespace std;

int main () {

    unsigned period_size = 2048;
    unsigned write_periods = 100;
    Core core;

    //TODO core.setSamplingRate(sr);

    core.setPeriodSize(period_size);

    core.setProcessor(processorType::NOOPERATION);
    core.setOutput(outputType::FILE);

    core.addGenerator(generatorType::PRIMITIVE);
    core.addSource();

    core.render(write_periods);

    return 1;
}
