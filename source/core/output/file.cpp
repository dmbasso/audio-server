#include "file.h"
#include "../wav_file.h"

#include <iostream>

using namespace std;

namespace aserver {
namespace output {

File::File()
{
    FileOutputConfigData *cfgData = new FileOutputConfigData();
    cfgData->flags = fileConfigFlags::FILE_ALL;
    config(cfgData);
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

void File::write(SoundBuffer &buffer)
{
    if (!fs.is_open()) {
        fs.open(outputFilePath, fstream::out | fstream::in | fstream::trunc);
        if (fs.is_open()) {
            writeWavHeader(&fs, 0);
        }
        else {
            cout << "Failed to open output file..." << endl;
        }
    }

    fs.write(reinterpret_cast<char *>(buffer.getData()), 2 * 2 * buffer.getPeriodSize());
    currentSize += 2 * 2 * buffer.getPeriodSize();
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
