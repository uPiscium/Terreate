#include <audio/ALhelper.hpp>
#include <audio/dynamic.hpp>
#include <exceptions.hpp>

#include <filesystem>
#include <iostream>

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

Bool Initialize() {
  Str libpath = OPENAL_LIB_PATH;
  ALModule = LoadDynamic(libpath);

  if (ALModule == nullptr) {
    ALModule =
        LoadDynamic(std::filesystem::current_path().string() + "/" + libpath);
  }

  if (ALModule == nullptr) {
    Str msg = Str("Failed to load OpenAL library: ") + libpath;
    throw Exceptions::OpenALException(msg);
  }

  ALCCREATECONTEXT =
      ExtractFunction<FPALCCREATECONTEXT>(ALModule, "alcCreateContext");
  ALCMAKECONTEXTCURRENT = ExtractFunction<FPALCMAKECONTEXTCURRENT>(
      ALModule, "alcMakeContextCurrent");
  ALCPROCESSCONTEXT =
      ExtractFunction<FPALCPROCESSCONTEXT>(ALModule, "alcProcessContext");
  ALCSUSPENDCONTEXT =
      ExtractFunction<FPALCSUSPENDCONTEXT>(ALModule, "alcSuspendContext");
  ALCDESTROYCONTEXT =
      ExtractFunction<FPALCDESTROYCONTEXT>(ALModule, "alcDestroyContext");
  ALCGETCURRENTCONTEXT =
      ExtractFunction<FPALCGETCURRENTCONTEXT>(ALModule, "alcGetCurrentContext");
  ALCGETCONTEXTSDEVICE =
      ExtractFunction<FPALCGETCONTEXTSDEVICE>(ALModule, "alcGetContextsDevice");

  ALCOPENDEVICE = ExtractFunction<FPALCOPENDEVICE>(ALModule, "alcOpenDevice");
  ALCCLOSEDEVICE =
      ExtractFunction<FPALCCLOSEDEVICE>(ALModule, "alcCloseDevice");

  ALCGETERROR = ExtractFunction<FPALCGETERROR>(ALModule, "alcGetError");

  ALCISEXTENSIONPRESENT = ExtractFunction<FPALCISEXTENSIONPRESENT>(
      ALModule, "alcIsExtensionPresent");
  ALCGETPROCADDRESS =
      ExtractFunction<FPALCGETPROCADDRESS>(ALModule, "alcGetProcaddress");
  ALCGETENUMVALUE =
      ExtractFunction<FPALCGETENUMVALUE>(ALModule, "alcGetEnumValue");

  ALCGETSTRING = ExtractFunction<FPALCGETSTRING>(ALModule, "alcGetString");
  ALCGETINTEGERV =
      ExtractFunction<FPALCGETINTEGERV>(ALModule, "alcGetIntegerv");

  ALCCAPTUREOPENDEVICE =
      ExtractFunction<FPALCCAPTUREOPENDEVICE>(ALModule, "alcCaptureOpenDevice");
  ALCCAPTURECLOSEDEVICE = ExtractFunction<FPALCCAPTURECLOSEDEVICE>(
      ALModule, "alcCaptureCloseDevice");
  ALCCAPTURESTART =
      ExtractFunction<FPALCCAPTURESTART>(ALModule, "alcCaptureStart");
  ALCCAPTURESTOP =
      ExtractFunction<FPALCCAPTURESTOP>(ALModule, "alcCaptureStop");
  ALCCAPTURESAMPLES =
      ExtractFunction<FPALCCAPTURESAMPLES>(ALModule, "alcCaptureSamples");

  ALENABLE = ExtractFunction<FPALENABLE>(ALModule, "alEnable");
  ALDISABLE = ExtractFunction<FPALDISABLE>(ALModule, "alDisable");
  ALISENABLED = ExtractFunction<FPALISENABLED>(ALModule, "alIsEnabled");

  ALGETSTRING = ExtractFunction<FPALGETSTRING>(ALModule, "alGetString");
  ALGETBOOLEANV = ExtractFunction<FPALGETBOOLEANV>(ALModule, "alGetBooleanv");
  ALGETINTEGERV = ExtractFunction<FPALGETINTEGERV>(ALModule, "alGetIntegerv");
  ALGETFLOATV = ExtractFunction<FPALGETFLOATV>(ALModule, "alGetFloatv");
  ALGETDOUBLEV = ExtractFunction<FPALGETDOUBLEV>(ALModule, "alGetDoublev");
  ALGETBOOLEAN = ExtractFunction<FPALGETBOOLEAN>(ALModule, "alGetBoolean");
  ALGETINTEGER = ExtractFunction<FPALGETINTEGER>(ALModule, "alGetInteger");
  ALGETFLOAT = ExtractFunction<FPALGETFLOAT>(ALModule, "alGetFloat");
  ALGETDOUBLE = ExtractFunction<FPALGETDOUBLE>(ALModule, "alGetDouble");

  ALGETERROR = ExtractFunction<FPALGETERROR>(ALModule, "alGetError");

  ALISEXTENSIONPRESENT =
      ExtractFunction<FPALISEXTENSIONPRESENT>(ALModule, "alIsExtensionPresent");
  ALGETPROCADDRESS =
      ExtractFunction<FPALGETPROCADDRESS>(ALModule, "alGetProcaddress");
  ALGETENUMVALUE =
      ExtractFunction<FPALGETENUMVALUE>(ALModule, "alGetEnumValue");

  ALLISTENERF = ExtractFunction<FPALLISTENERF>(ALModule, "alListenerf");
  ALLISTENER3F = ExtractFunction<FPALLISTENER3F>(ALModule, "alListener3f");
  ALLISTENERFV = ExtractFunction<FPALLISTENERFV>(ALModule, "alListenerfv");
  ALLISTENERI = ExtractFunction<FPALLISTENERI>(ALModule, "alListeneri");
  ALLISTENER3I = ExtractFunction<FPALLISTENER3I>(ALModule, "alListener3i");
  ALLISTENERIV = ExtractFunction<FPALLISTENERIV>(ALModule, "alListeneriv");

  ALGETLISTENERF =
      ExtractFunction<FPALGETLISTENERF>(ALModule, "alGetListenerf");
  ALGETLISTENER3F =
      ExtractFunction<FPALGETLISTENER3F>(ALModule, "alGetListener3f");
  ALGETLISTENERFV =
      ExtractFunction<FPALGETLISTENERFV>(ALModule, "alGetListenerfv");
  ALGETLISTENERI =
      ExtractFunction<FPALGETLISTENERI>(ALModule, "alGetListeneri");
  ALGETLISTENER3I =
      ExtractFunction<FPALGETLISTENER3I>(ALModule, "alGetListener3i");
  ALGETLISTENERIV =
      ExtractFunction<FPALGETLISTENERIV>(ALModule, "alGetListeneriv");

  ALGENSOURCES = ExtractFunction<FPALGENSOURCES>(ALModule, "alGenSources");
  ALDELETESOURCES =
      ExtractFunction<FPALDELETESOURCES>(ALModule, "alDeleteSources");
  ALISSOURCE = ExtractFunction<FPALISSOURCE>(ALModule, "alIsSource");

  ALSOURCEF = ExtractFunction<FPALSOURCEF>(ALModule, "alSourcef");
  ALSOURCE3F = ExtractFunction<FPALSOURCE3F>(ALModule, "alSource3f");
  ALSOURCEFV = ExtractFunction<FPALSOURCEFV>(ALModule, "alSourcefv");
  ALSOURCEI = ExtractFunction<FPALSOURCEI>(ALModule, "alSourcei");
  ALSOURCE3I = ExtractFunction<FPALSOURCE3I>(ALModule, "alSource3i");
  ALSOURCEIV = ExtractFunction<FPALSOURCEIV>(ALModule, "alSourceiv");

  ALGETSOURCEF = ExtractFunction<FPALGETSOURCEF>(ALModule, "alGetSourcef");
  ALGETSOURCE3F = ExtractFunction<FPALGETSOURCE3F>(ALModule, "alGetSource3f");
  ALGETSOURCEFV = ExtractFunction<FPALGETSOURCEFV>(ALModule, "alGetSourcefv");
  ALGETSOURCEI = ExtractFunction<FPALGETSOURCEI>(ALModule, "alGetSourcei");
  ALGETSOURCE3I = ExtractFunction<FPALGETSOURCE3I>(ALModule, "alGetSource3i");
  ALGETSOURCEIV = ExtractFunction<FPALGETSOURCEIV>(ALModule, "alGetSourceiv");

  ALSOURCEPLAYV = ExtractFunction<FPALSOURCEPLAYV>(ALModule, "alSourcePlayv");
  ALSOURCESTOPV = ExtractFunction<FPALSOURCESTOPV>(ALModule, "alSourceStopv");
  ALSOURCEREWINDV =
      ExtractFunction<FPALSOURCEREWINDV>(ALModule, "alSourcereWindv");
  ALSOURCEPAUSEV =
      ExtractFunction<FPALSOURCEPAUSEV>(ALModule, "alSourcePausev");

  ALSOURCEPLAY = ExtractFunction<FPALSOURCEPLAY>(ALModule, "alSourcePlay");
  ALSOURCESTOP = ExtractFunction<FPALSOURCESTOP>(ALModule, "alSourceStop");
  ALSOURCEREWIND =
      ExtractFunction<FPALSOURCEREWIND>(ALModule, "alSourcereWind");
  ALSOURCEPAUSE = ExtractFunction<FPALSOURCEPAUSE>(ALModule, "alSourcePause");

  ALSOURCEQUEUEBUFFERS =
      ExtractFunction<FPALSOURCEQUEUEBUFFERS>(ALModule, "alSourceQueueBuffers");
  ALSOURCEUNQUEUEBUFFERS = ExtractFunction<FPALSOURCEUNQUEUEBUFFERS>(
      ALModule, "alSourceUnqueueBuffers");

  ALGENBUFFERS = ExtractFunction<FPALGENBUFFERS>(ALModule, "alGenBuffers");
  ALDELETEBUFFERS =
      ExtractFunction<FPALDELETEBUFFERS>(ALModule, "alDeleteBuffers");
  ALISBUFFER = ExtractFunction<FPALISBUFFER>(ALModule, "alIsBuffer");
  ALBUFFERDATA = ExtractFunction<FPALBUFFERDATA>(ALModule, "alBufferData");

  ALBUFFERF = ExtractFunction<FPALBUFFERF>(ALModule, "alBufferf");
  ALBUFFER3F = ExtractFunction<FPALBUFFER3F>(ALModule, "alBuffer3f");
  ALBUFFERFV = ExtractFunction<FPALBUFFERFV>(ALModule, "alBufferfv");
  ALBUFFERI = ExtractFunction<FPALBUFFERI>(ALModule, "alBufferi");
  ALBUFFER3I = ExtractFunction<FPALBUFFER3I>(ALModule, "alBuffer3i");
  ALBUFFERIV = ExtractFunction<FPALBUFFERIV>(ALModule, "alBufferiv");

  ALGETBUFFERF = ExtractFunction<FPALGETBUFFERF>(ALModule, "alGetBufferf");
  ALGETBUFFER3F = ExtractFunction<FPALGETBUFFER3F>(ALModule, "alGetBuffer3f");
  ALGETBUFFERFV = ExtractFunction<FPALGETBUFFERFV>(ALModule, "alGetBufferfv");
  ALGETBUFFERI = ExtractFunction<FPALGETBUFFERI>(ALModule, "alGetBufferi");
  ALGETBUFFER3I = ExtractFunction<FPALGETBUFFER3I>(ALModule, "alGetBuffer3i");
  ALGETBUFFERIV = ExtractFunction<FPALGETBUFFERIV>(ALModule, "alGetBufferiv");

  ALDOPPLERFACTOR =
      ExtractFunction<FPALDOPPLERFACTOR>(ALModule, "alDopplerFactor");
  ALSPEEDOFSOUND =
      ExtractFunction<FPALSPEEDOFSOUND>(ALModule, "alSpeedofSound");
  ALDISTANCEMODEL =
      ExtractFunction<FPALDISTANCEMODEL>(ALModule, "alDistanceModel");

  return true;
}

void Terminate() {
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

  FreeDynamic(ALModule);
}

} // namespace Terreate::Audio::ALhelper
