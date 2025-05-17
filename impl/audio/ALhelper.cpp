#include <filesystem>
#include <iostream>

#include <audio/ALhelper.hpp>
#include <audio/dynamic.hpp>
#include <core/exception.hpp>

namespace Terreate::Audio::ALhelper {
using namespace Terreate::Audio::Dynamic;

Module ALModule = nullptr;

FPALCCREATECONTEXT ALCCREATECONTEXT = nullptr;
FPALCMAKECONTEXTCURRENT ALCMAKECONTEXTCURRENT = nullptr;
FPALCPROCESSCONTEXT ALCPROCESSCONTEXT = nullptr;
FPALCSUSPENDCONTEXT ALCSUSPENDCONTEXT = nullptr;
FPALCDESTROYCONTEXT ALCDESTROYCONTEXT = nullptr;
FPALCGETCURRENTCONTEXT ALCGETCURRENTCONTEXT = nullptr;
FPALCGETCONTEXTSDEVICE ALCGETCONTEXTSDEVICE = nullptr;

FPALCOPENDEVICE ALCOPENDEVICE = nullptr;
FPALCCLOSEDEVICE ALCCLOSEDEVICE = nullptr;

FPALCGETERROR ALCGETERROR = nullptr;

FPALCISEXTENSIONPRESENT ALCISEXTENSIONPRESENT = nullptr;
FPALCGETPROCADDRESS ALCGETPROCADDRESS = nullptr;
FPALCGETENUMVALUE ALCGETENUMVALUE = nullptr;

FPALCGETSTRING ALCGETSTRING = nullptr;
FPALCGETINTEGERV ALCGETINTEGERV = nullptr;

FPALCCAPTUREOPENDEVICE ALCCAPTUREOPENDEVICE = nullptr;
FPALCCAPTURECLOSEDEVICE ALCCAPTURECLOSEDEVICE = nullptr;
FPALCCAPTURESTART ALCCAPTURESTART = nullptr;
FPALCCAPTURESTOP ALCCAPTURESTOP = nullptr;
FPALCCAPTURESAMPLES ALCCAPTURESAMPLES = nullptr;

FPALENABLE ALENABLE = nullptr;
FPALDISABLE ALDISABLE = nullptr;
FPALISENABLED ALISENABLED = nullptr;

FPALGETSTRING ALGETSTRING = nullptr;
FPALGETBOOLEANV ALGETBOOLEANV = nullptr;
FPALGETINTEGERV ALGETINTEGERV = nullptr;
FPALGETFLOATV ALGETFLOATV = nullptr;
FPALGETDOUBLEV ALGETDOUBLEV = nullptr;
FPALGETBOOLEAN ALGETBOOLEAN = nullptr;
FPALGETINTEGER ALGETINTEGER = nullptr;
FPALGETFLOAT ALGETFLOAT = nullptr;
FPALGETDOUBLE ALGETDOUBLE = nullptr;

FPALGETERROR ALGETERROR = nullptr;

FPALISEXTENSIONPRESENT ALISEXTENSIONPRESENT = nullptr;
FPALGETPROCADDRESS ALGETPROCADDRESS = nullptr;
FPALGETENUMVALUE ALGETENUMVALUE = nullptr;

FPALLISTENERF ALLISTENERF = nullptr;
FPALLISTENER3F ALLISTENER3F = nullptr;
FPALLISTENERFV ALLISTENERFV = nullptr;
FPALLISTENERI ALLISTENERI = nullptr;
FPALLISTENER3I ALLISTENER3I = nullptr;
FPALLISTENERIV ALLISTENERIV = nullptr;

FPALGETLISTENERF ALGETLISTENERF = nullptr;
FPALGETLISTENER3F ALGETLISTENER3F = nullptr;
FPALGETLISTENERFV ALGETLISTENERFV = nullptr;
FPALGETLISTENERI ALGETLISTENERI = nullptr;
FPALGETLISTENER3I ALGETLISTENER3I = nullptr;
FPALGETLISTENERIV ALGETLISTENERIV = nullptr;

FPALGENSOURCES ALGENSOURCES = nullptr;
FPALDELETESOURCES ALDELETESOURCES = nullptr;
FPALISSOURCE ALISSOURCE = nullptr;

FPALSOURCEF ALSOURCEF = nullptr;
FPALSOURCE3F ALSOURCE3F = nullptr;
FPALSOURCEFV ALSOURCEFV = nullptr;
FPALSOURCEI ALSOURCEI = nullptr;
FPALSOURCE3I ALSOURCE3I = nullptr;
FPALSOURCEIV ALSOURCEIV = nullptr;

FPALGETSOURCEF ALGETSOURCEF = nullptr;
FPALGETSOURCE3F ALGETSOURCE3F = nullptr;
FPALGETSOURCEFV ALGETSOURCEFV = nullptr;
FPALGETSOURCEI ALGETSOURCEI = nullptr;
FPALGETSOURCE3I ALGETSOURCE3I = nullptr;
FPALGETSOURCEIV ALGETSOURCEIV = nullptr;

FPALSOURCEPLAYV ALSOURCEPLAYV = nullptr;
FPALSOURCESTOPV ALSOURCESTOPV = nullptr;
FPALSOURCEREWINDV ALSOURCEREWINDV = nullptr;
FPALSOURCEPAUSEV ALSOURCEPAUSEV = nullptr;

FPALSOURCEPLAY ALSOURCEPLAY = nullptr;
FPALSOURCESTOP ALSOURCESTOP = nullptr;
FPALSOURCEREWIND ALSOURCEREWIND = nullptr;
FPALSOURCEPAUSE ALSOURCEPAUSE = nullptr;

FPALSOURCEQUEUEBUFFERS ALSOURCEQUEUEBUFFERS = nullptr;
FPALSOURCEUNQUEUEBUFFERS ALSOURCEUNQUEUEBUFFERS = nullptr;

FPALGENBUFFERS ALGENBUFFERS = nullptr;
FPALDELETEBUFFERS ALDELETEBUFFERS = nullptr;
FPALISBUFFER ALISBUFFER = nullptr;
FPALBUFFERDATA ALBUFFERDATA = nullptr;

FPALBUFFERF ALBUFFERF = nullptr;
FPALBUFFER3F ALBUFFER3F = nullptr;
FPALBUFFERFV ALBUFFERFV = nullptr;
FPALBUFFERI ALBUFFERI = nullptr;
FPALBUFFER3I ALBUFFER3I = nullptr;
FPALBUFFERIV ALBUFFERIV = nullptr;

FPALGETBUFFERF ALGETBUFFERF = nullptr;
FPALGETBUFFER3F ALGETBUFFER3F = nullptr;
FPALGETBUFFERFV ALGETBUFFERFV = nullptr;
FPALGETBUFFERI ALGETBUFFERI = nullptr;
FPALGETBUFFER3I ALGETBUFFER3I = nullptr;
FPALGETBUFFERIV ALGETBUFFERIV = nullptr;

FPALDOPPLERFACTOR ALDOPPLERFACTOR = nullptr;
FPALSPEEDOFSOUND ALSPEEDOFSOUND = nullptr;
FPALDISTANCEMODEL ALDISTANCEMODEL = nullptr;

bool initialize() {
  Str libpath = OPENAL_LIB_PATH;
  ALModule = loadDynamic(libpath);

  if (ALModule == nullptr) {
    ALModule =
        loadDynamic(std::filesystem::current_path().string() + "/" + libpath);
  }

  if (ALModule == nullptr) {
    Str msg = Str("Failed to load OpenAL library: ") + libpath;
    throw Exceptions::OpenALException(msg);
  }

  ALCCREATECONTEXT =
      extractFunction<FPALCCREATECONTEXT>(ALModule, "alcCreateContext");
  ALCMAKECONTEXTCURRENT = extractFunction<FPALCMAKECONTEXTCURRENT>(
      ALModule, "alcMakeContextCurrent");
  ALCPROCESSCONTEXT =
      extractFunction<FPALCPROCESSCONTEXT>(ALModule, "alcProcessContext");
  ALCSUSPENDCONTEXT =
      extractFunction<FPALCSUSPENDCONTEXT>(ALModule, "alcSuspendContext");
  ALCDESTROYCONTEXT =
      extractFunction<FPALCDESTROYCONTEXT>(ALModule, "alcDestroyContext");
  ALCGETCURRENTCONTEXT =
      extractFunction<FPALCGETCURRENTCONTEXT>(ALModule, "alcGetCurrentContext");
  ALCGETCONTEXTSDEVICE =
      extractFunction<FPALCGETCONTEXTSDEVICE>(ALModule, "alcGetContextsDevice");

  ALCOPENDEVICE = extractFunction<FPALCOPENDEVICE>(ALModule, "alcOpenDevice");
  ALCCLOSEDEVICE =
      extractFunction<FPALCCLOSEDEVICE>(ALModule, "alcCloseDevice");

  ALCGETERROR = extractFunction<FPALCGETERROR>(ALModule, "alcGetError");

  ALCISEXTENSIONPRESENT = extractFunction<FPALCISEXTENSIONPRESENT>(
      ALModule, "alcIsExtensionPresent");
  ALCGETPROCADDRESS =
      extractFunction<FPALCGETPROCADDRESS>(ALModule, "alcGetProcaddress");
  ALCGETENUMVALUE =
      extractFunction<FPALCGETENUMVALUE>(ALModule, "alcGetEnumValue");

  ALCGETSTRING = extractFunction<FPALCGETSTRING>(ALModule, "alcGetString");
  ALCGETINTEGERV =
      extractFunction<FPALCGETINTEGERV>(ALModule, "alcGetIntegerv");

  ALCCAPTUREOPENDEVICE =
      extractFunction<FPALCCAPTUREOPENDEVICE>(ALModule, "alcCaptureOpenDevice");
  ALCCAPTURECLOSEDEVICE = extractFunction<FPALCCAPTURECLOSEDEVICE>(
      ALModule, "alcCaptureCloseDevice");
  ALCCAPTURESTART =
      extractFunction<FPALCCAPTURESTART>(ALModule, "alcCaptureStart");
  ALCCAPTURESTOP =
      extractFunction<FPALCCAPTURESTOP>(ALModule, "alcCaptureStop");
  ALCCAPTURESAMPLES =
      extractFunction<FPALCCAPTURESAMPLES>(ALModule, "alcCaptureSamples");

  ALENABLE = extractFunction<FPALENABLE>(ALModule, "alEnable");
  ALDISABLE = extractFunction<FPALDISABLE>(ALModule, "alDisable");
  ALISENABLED = extractFunction<FPALISENABLED>(ALModule, "alIsEnabled");

  ALGETSTRING = extractFunction<FPALGETSTRING>(ALModule, "alGetString");
  ALGETBOOLEANV = extractFunction<FPALGETBOOLEANV>(ALModule, "alGetBooleanv");
  ALGETINTEGERV = extractFunction<FPALGETINTEGERV>(ALModule, "alGetIntegerv");
  ALGETFLOATV = extractFunction<FPALGETFLOATV>(ALModule, "alGetFloatv");
  ALGETDOUBLEV = extractFunction<FPALGETDOUBLEV>(ALModule, "alGetDoublev");
  ALGETBOOLEAN = extractFunction<FPALGETBOOLEAN>(ALModule, "alGetBoolean");
  ALGETINTEGER = extractFunction<FPALGETINTEGER>(ALModule, "alGetInteger");
  ALGETFLOAT = extractFunction<FPALGETFLOAT>(ALModule, "alGetFloat");
  ALGETDOUBLE = extractFunction<FPALGETDOUBLE>(ALModule, "alGetDouble");

  ALGETERROR = extractFunction<FPALGETERROR>(ALModule, "alGetError");

  ALISEXTENSIONPRESENT =
      extractFunction<FPALISEXTENSIONPRESENT>(ALModule, "alIsExtensionPresent");
  ALGETPROCADDRESS =
      extractFunction<FPALGETPROCADDRESS>(ALModule, "alGetProcaddress");
  ALGETENUMVALUE =
      extractFunction<FPALGETENUMVALUE>(ALModule, "alGetEnumValue");

  ALLISTENERF = extractFunction<FPALLISTENERF>(ALModule, "alListenerf");
  ALLISTENER3F = extractFunction<FPALLISTENER3F>(ALModule, "alListener3f");
  ALLISTENERFV = extractFunction<FPALLISTENERFV>(ALModule, "alListenerfv");
  ALLISTENERI = extractFunction<FPALLISTENERI>(ALModule, "alListeneri");
  ALLISTENER3I = extractFunction<FPALLISTENER3I>(ALModule, "alListener3i");
  ALLISTENERIV = extractFunction<FPALLISTENERIV>(ALModule, "alListeneriv");

  ALGETLISTENERF =
      extractFunction<FPALGETLISTENERF>(ALModule, "alGetListenerf");
  ALGETLISTENER3F =
      extractFunction<FPALGETLISTENER3F>(ALModule, "alGetListener3f");
  ALGETLISTENERFV =
      extractFunction<FPALGETLISTENERFV>(ALModule, "alGetListenerfv");
  ALGETLISTENERI =
      extractFunction<FPALGETLISTENERI>(ALModule, "alGetListeneri");
  ALGETLISTENER3I =
      extractFunction<FPALGETLISTENER3I>(ALModule, "alGetListener3i");
  ALGETLISTENERIV =
      extractFunction<FPALGETLISTENERIV>(ALModule, "alGetListeneriv");

  ALGENSOURCES = extractFunction<FPALGENSOURCES>(ALModule, "alGenSources");
  ALDELETESOURCES =
      extractFunction<FPALDELETESOURCES>(ALModule, "alDeleteSources");
  ALISSOURCE = extractFunction<FPALISSOURCE>(ALModule, "alIsSource");

  ALSOURCEF = extractFunction<FPALSOURCEF>(ALModule, "alSourcef");
  ALSOURCE3F = extractFunction<FPALSOURCE3F>(ALModule, "alSource3f");
  ALSOURCEFV = extractFunction<FPALSOURCEFV>(ALModule, "alSourcefv");
  ALSOURCEI = extractFunction<FPALSOURCEI>(ALModule, "alSourcei");
  ALSOURCE3I = extractFunction<FPALSOURCE3I>(ALModule, "alSource3i");
  ALSOURCEIV = extractFunction<FPALSOURCEIV>(ALModule, "alSourceiv");

  ALGETSOURCEF = extractFunction<FPALGETSOURCEF>(ALModule, "alGetSourcef");
  ALGETSOURCE3F = extractFunction<FPALGETSOURCE3F>(ALModule, "alGetSource3f");
  ALGETSOURCEFV = extractFunction<FPALGETSOURCEFV>(ALModule, "alGetSourcefv");
  ALGETSOURCEI = extractFunction<FPALGETSOURCEI>(ALModule, "alGetSourcei");
  ALGETSOURCE3I = extractFunction<FPALGETSOURCE3I>(ALModule, "alGetSource3i");
  ALGETSOURCEIV = extractFunction<FPALGETSOURCEIV>(ALModule, "alGetSourceiv");

  ALSOURCEPLAYV = extractFunction<FPALSOURCEPLAYV>(ALModule, "alSourcePlayv");
  ALSOURCESTOPV = extractFunction<FPALSOURCESTOPV>(ALModule, "alSourceStopv");
  ALSOURCEREWINDV =
      extractFunction<FPALSOURCEREWINDV>(ALModule, "alSourcereWindv");
  ALSOURCEPAUSEV =
      extractFunction<FPALSOURCEPAUSEV>(ALModule, "alSourcePausev");

  ALSOURCEPLAY = extractFunction<FPALSOURCEPLAY>(ALModule, "alSourcePlay");
  ALSOURCESTOP = extractFunction<FPALSOURCESTOP>(ALModule, "alSourceStop");
  ALSOURCEREWIND =
      extractFunction<FPALSOURCEREWIND>(ALModule, "alSourcereWind");
  ALSOURCEPAUSE = extractFunction<FPALSOURCEPAUSE>(ALModule, "alSourcePause");

  ALSOURCEQUEUEBUFFERS =
      extractFunction<FPALSOURCEQUEUEBUFFERS>(ALModule, "alSourceQueueBuffers");
  ALSOURCEUNQUEUEBUFFERS = extractFunction<FPALSOURCEUNQUEUEBUFFERS>(
      ALModule, "alSourceUnqueueBuffers");

  ALGENBUFFERS = extractFunction<FPALGENBUFFERS>(ALModule, "alGenBuffers");
  ALDELETEBUFFERS =
      extractFunction<FPALDELETEBUFFERS>(ALModule, "alDeleteBuffers");
  ALISBUFFER = extractFunction<FPALISBUFFER>(ALModule, "alIsBuffer");
  ALBUFFERDATA = extractFunction<FPALBUFFERDATA>(ALModule, "alBufferData");

  ALBUFFERF = extractFunction<FPALBUFFERF>(ALModule, "alBufferf");
  ALBUFFER3F = extractFunction<FPALBUFFER3F>(ALModule, "alBuffer3f");
  ALBUFFERFV = extractFunction<FPALBUFFERFV>(ALModule, "alBufferfv");
  ALBUFFERI = extractFunction<FPALBUFFERI>(ALModule, "alBufferi");
  ALBUFFER3I = extractFunction<FPALBUFFER3I>(ALModule, "alBuffer3i");
  ALBUFFERIV = extractFunction<FPALBUFFERIV>(ALModule, "alBufferiv");

  ALGETBUFFERF = extractFunction<FPALGETBUFFERF>(ALModule, "alGetBufferf");
  ALGETBUFFER3F = extractFunction<FPALGETBUFFER3F>(ALModule, "alGetBuffer3f");
  ALGETBUFFERFV = extractFunction<FPALGETBUFFERFV>(ALModule, "alGetBufferfv");
  ALGETBUFFERI = extractFunction<FPALGETBUFFERI>(ALModule, "alGetBufferi");
  ALGETBUFFER3I = extractFunction<FPALGETBUFFER3I>(ALModule, "alGetBuffer3i");
  ALGETBUFFERIV = extractFunction<FPALGETBUFFERIV>(ALModule, "alGetBufferiv");

  ALDOPPLERFACTOR =
      extractFunction<FPALDOPPLERFACTOR>(ALModule, "alDopplerFactor");
  ALSPEEDOFSOUND =
      extractFunction<FPALSPEEDOFSOUND>(ALModule, "alSpeedofSound");
  ALDISTANCEMODEL =
      extractFunction<FPALDISTANCEMODEL>(ALModule, "alDistanceModel");

  return true;
}

void terminate() {
  ALCCREATECONTEXT = nullptr;
  ALCMAKECONTEXTCURRENT = nullptr;
  ALCPROCESSCONTEXT = nullptr;
  ALCSUSPENDCONTEXT = nullptr;
  ALCDESTROYCONTEXT = nullptr;
  ALCGETCURRENTCONTEXT = nullptr;
  ALCGETCONTEXTSDEVICE = nullptr;

  ALCOPENDEVICE = nullptr;
  ALCCLOSEDEVICE = nullptr;

  ALCGETERROR = nullptr;

  ALCISEXTENSIONPRESENT = nullptr;
  ALCGETPROCADDRESS = nullptr;
  ALCGETENUMVALUE = nullptr;

  ALCGETSTRING = nullptr;
  ALCGETINTEGERV = nullptr;

  ALCCAPTUREOPENDEVICE = nullptr;
  ALCCAPTURECLOSEDEVICE = nullptr;
  ALCCAPTURESTART = nullptr;
  ALCCAPTURESTOP = nullptr;
  ALCCAPTURESAMPLES = nullptr;

  ALENABLE = nullptr;
  ALDISABLE = nullptr;
  ALISENABLED = nullptr;

  ALGETSTRING = nullptr;
  ALGETBOOLEANV = nullptr;
  ALGETINTEGERV = nullptr;
  ALGETFLOATV = nullptr;
  ALGETDOUBLEV = nullptr;
  ALGETBOOLEAN = nullptr;
  ALGETINTEGER = nullptr;
  ALGETFLOAT = nullptr;
  ALGETDOUBLE = nullptr;

  ALGETERROR = nullptr;

  ALISEXTENSIONPRESENT = nullptr;
  ALGETPROCADDRESS = nullptr;
  ALGETENUMVALUE = nullptr;

  ALLISTENERF = nullptr;
  ALLISTENER3F = nullptr;
  ALLISTENERFV = nullptr;
  ALLISTENERI = nullptr;
  ALLISTENER3I = nullptr;
  ALLISTENERIV = nullptr;

  ALGETLISTENERF = nullptr;
  ALGETLISTENER3F = nullptr;
  ALGETLISTENERFV = nullptr;
  ALGETLISTENERI = nullptr;
  ALGETLISTENER3I = nullptr;
  ALGETLISTENERIV = nullptr;

  ALGENSOURCES = nullptr;
  ALDELETESOURCES = nullptr;
  ALISSOURCE = nullptr;

  ALSOURCEF = nullptr;
  ALSOURCE3F = nullptr;
  ALSOURCEFV = nullptr;
  ALSOURCEI = nullptr;
  ALSOURCE3I = nullptr;
  ALSOURCEIV = nullptr;

  ALGETSOURCEF = nullptr;
  ALGETSOURCE3F = nullptr;
  ALGETSOURCEFV = nullptr;
  ALGETSOURCEI = nullptr;
  ALGETSOURCE3I = nullptr;
  ALGETSOURCEIV = nullptr;

  ALSOURCEPLAYV = nullptr;
  ALSOURCESTOPV = nullptr;
  ALSOURCEREWINDV = nullptr;
  ALSOURCEPAUSEV = nullptr;

  ALSOURCEPLAY = nullptr;
  ALSOURCESTOP = nullptr;
  ALSOURCEREWIND = nullptr;
  ALSOURCEPAUSE = nullptr;

  ALSOURCEQUEUEBUFFERS = nullptr;
  ALSOURCEUNQUEUEBUFFERS = nullptr;

  ALGENBUFFERS = nullptr;
  ALDELETEBUFFERS = nullptr;
  ALISBUFFER = nullptr;
  ALBUFFERDATA = nullptr;

  ALBUFFERF = nullptr;
  ALBUFFER3F = nullptr;
  ALBUFFERFV = nullptr;
  ALBUFFERI = nullptr;
  ALBUFFER3I = nullptr;
  ALBUFFERIV = nullptr;

  ALGETBUFFERF = nullptr;
  ALGETBUFFER3F = nullptr;
  ALGETBUFFERFV = nullptr;
  ALGETBUFFERI = nullptr;
  ALGETBUFFER3I = nullptr;
  ALGETBUFFERIV = nullptr;

  ALDOPPLERFACTOR = nullptr;
  ALSPEEDOFSOUND = nullptr;
  ALDISTANCEMODEL = nullptr;

  freeDynamic(ALModule);
}

} // namespace Terreate::Audio::ALhelper
