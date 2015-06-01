#include "file.h"
#include "../wav_file.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace output {

File::File(Core *core, uint32_t periodSize) : Output(core, periodSize)
{
    FileOutputConfigData *cfgData = new FileOutputConfigData();
    cfgData->flags = fileConfigFlags::FILE_ALL;
    config(cfgData);
    currentSize = 0;
}

void File::config(ConfigData *configData)
{
    FileOutputConfigData *cfgData = (FileOutputConfigData *) configData;

    if(cfgData->flags & fileConfigFlags::OUTPUT_FILEPATH) {
        outputFilePath = cfgData->outputFilePath;
    }
    if(cfgData->flags & fileConfigFlags::NORMALISE) {
        normalise_audio = cfgData->normalise_audio;
    }
}

void File::write(SoundBuffer *buffer)
{
    if (!fs.is_open()) {
        fs.open(outputFilePath, fstream::out | fstream::in | fstream::trunc);
        if (fs.is_open()) {
            writeWavHeader(&fs, 0);
        } else {
            cout << "Failed to open output file..." << endl;
        }
    }

    if (buffer) {
        fs.write(reinterpret_cast<char *>(buffer->getData()), 2 * 2 * buffer->getPeriodSize());
    } else {
        fs.write(reinterpret_cast<char *>(silence->getData()), 2 * 2 * silence->getPeriodSize());
    }
    currentSize += 2 * 2 * silence->getPeriodSize();
}

void File::close()
{
    writeWavHeader(&fs, currentSize);
    if (normalise_audio) {
        normalise(&fs, currentSize);
    }
    fs.close();
}

} //end namespace output
} //end namespace aserver
