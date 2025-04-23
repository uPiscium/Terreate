#ifndef __TERREATEAUDIO_ALHELPER_HPP__
#define __TERREATEAUDIO_ALHELPER_HPP__

#include <types.hpp>

#define AL_NO_PROTOTYPES
#include <openal/al.h>
#include <openal/alc.h>

#define TERREATE_AUDIO_NAMESPACE Terreate::Audio::ALhelper

#define alcCreateContext TERREATE_AUDIO_NAMESPACE::ALCCREATECONTEXT
#define alcMakeContextCurrent TERREATE_AUDIO_NAMESPACE::ALCMAKECONTEXTCURRENT
#define alcProcessContext TERREATE_AUDIO_NAMESPACE::ALCPROCESSCONTEXT
#define alcSuspendContext TERREATE_AUDIO_NAMESPACE::ALCSUSPENDCONTEXT
#define alcDestroyContext TERREATE_AUDIO_NAMESPACE::ALCDESTROYCONTEXT
#define alcGetCurrentContext TERREATE_AUDIO_NAMESPACE::ALCGETCURRENTCONTEXT
#define alcGetContextsDevice TERREATE_AUDIO_NAMESPACE::ALCGETCONTEXTSDEVICE

#define alcOpenDevice TERREATE_AUDIO_NAMESPACE::ALCOPENDEVICE
#define alcCloseDevice TERREATE_AUDIO_NAMESPACE::ALCCLOSEDEVICE

#define alcGetError TERREATE_AUDIO_NAMESPACE::ALCGETERROR
#define alcIsExtensionPresent TERREATE_AUDIO_NAMESPACE::ALCISEXTENSIONPRESENT
#define alcGetProcaddress TERREATE_AUDIO_NAMESPACE::ALCGETPROCADDRESS
#define alcGetEnumValue TERREATE_AUDIO_NAMESPACE::ALCGETENUMVALUE

#define alcGetString TERREATE_AUDIO_NAMESPACE::ALCGETSTRING
#define alcGetIntegerv TERREATE_AUDIO_NAMESPACE::ALCGETINTEGERV

#define alcCaptureOpenDevice TERREATE_AUDIO_NAMESPACE::ALCCAPTUREOPENDEVICE
#define alcCaptureCloseDevice TERREATE_AUDIO_NAMESPACE::ALCCAPTURECLOSEDEVICE
#define alcCaptureStart TERREATE_AUDIO_NAMESPACE::ALCCAPTURESTART
#define alcCaptureStop TERREATE_AUDIO_NAMESPACE::ALCCAPTURESTOP
#define alcCaptureSamples TERREATE_AUDIO_NAMESPACE::ALCCAPTURESAMPLES

#define alEnable TERREATE_AUDIO_NAMESPACE::ALENABLE
#define alDisable TERREATE_AUDIO_NAMESPACE::ALDISABLE
#define alIsEnabled TERREATE_AUDIO_NAMESPACE::ALISENABLED

#define alGetString TERREATE_AUDIO_NAMESPACE::ALGETSTRING
#define alGetBooleanv TERREATE_AUDIO_NAMESPACE::ALGETBOOLEANV
#define alGetIntegerv TERREATE_AUDIO_NAMESPACE::ALGETINTEGERV
#define alGetFloatv TERREATE_AUDIO_NAMESPACE::ALGETFLOATV
#define alGetDoublev TERREATE_AUDIO_NAMESPACE::ALGETDOUBLEV
#define alGetBoolean TERREATE_AUDIO_NAMESPACE::ALGETBOOLEAN
#define alGetInteger TERREATE_AUDIO_NAMESPACE::ALGETINTEGER
#define alGetFloat TERREATE_AUDIO_NAMESPACE::ALGETFLOAT
#define alGetDouble TERREATE_AUDIO_NAMESPACE::ALGETDOUBLE

#define alGetError TERREATE_AUDIO_NAMESPACE::ALGETERROR

#define alIsExtensionPresent TERREATE_AUDIO_NAMESPACE::ALISEXTENSIONPRESENT
#define alGetProcaddress TERREATE_AUDIO_NAMESPACE::ALGETPROCADDRESS
#define alGetEnumValue TERREATE_AUDIO_NAMESPACE::ALGETENUMVALUE

#define alListenerf TERREATE_AUDIO_NAMESPACE::ALLISTENERF
#define alListener3f TERREATE_AUDIO_NAMESPACE::ALLISTENER3F
#define alListenerfv TERREATE_AUDIO_NAMESPACE::ALLISTENERFV
#define alListeneri TERREATE_AUDIO_NAMESPACE::ALLISTENERI
#define alListener3i TERREATE_AUDIO_NAMESPACE::ALLISTENER3I
#define alListeneriv TERREATE_AUDIO_NAMESPACE::ALLISTENERIV

#define alGetListenerf TERREATE_AUDIO_NAMESPACE::ALGETLISTENERF
#define alGetListener3f TERREATE_AUDIO_NAMESPACE::ALGETLISTENER3F
#define alGetListenerfv TERREATE_AUDIO_NAMESPACE::ALGETLISTENERFV
#define alGetListeneri TERREATE_AUDIO_NAMESPACE::ALGETLISTENERI
#define alGetListener3i TERREATE_AUDIO_NAMESPACE::ALGETLISTENER3I
#define alGetListeneriv TERREATE_AUDIO_NAMESPACE::ALGETLISTENERIV

#define alGenSources TERREATE_AUDIO_NAMESPACE::ALGENSOURCES
#define alDeleteSources TERREATE_AUDIO_NAMESPACE::ALDELETESOURCES
#define alIsSource TERREATE_AUDIO_NAMESPACE::ALISSOURCE

#define alSourcef TERREATE_AUDIO_NAMESPACE::ALSOURCEF
#define alSource3f TERREATE_AUDIO_NAMESPACE::ALSOURCE3F
#define alSourcefv TERREATE_AUDIO_NAMESPACE::ALSOURCEFV
#define alSourcei TERREATE_AUDIO_NAMESPACE::ALSOURCEI
#define alSource3i TERREATE_AUDIO_NAMESPACE::ALSOURCE3I
#define alSourceiv TERREATE_AUDIO_NAMESPACE::ALSOURCEIV

#define alGetSourcef TERREATE_AUDIO_NAMESPACE::ALGETSOURCEF
#define alGetSource3f TERREATE_AUDIO_NAMESPACE::ALGETSOURCE3F
#define alGetSourcefv TERREATE_AUDIO_NAMESPACE::ALGETSOURCEFV
#define alGetSourcei TERREATE_AUDIO_NAMESPACE::ALGETSOURCEI
#define alGetSource3i TERREATE_AUDIO_NAMESPACE::ALGETSOURCE3I
#define alGetSourceiv TERREATE_AUDIO_NAMESPACE::ALGETSOURCEIV

#define alSourcePlayv TERREATE_AUDIO_NAMESPACE::ALSOURCEPLAYV
#define alSourceStopv TERREATE_AUDIO_NAMESPACE::ALSOURCESTOPV
#define alSourcereWindv TERREATE_AUDIO_NAMESPACE::ALSOURCEREWINDV
#define alSourcePausev TERREATE_AUDIO_NAMESPACE::ALSOURCEPAUSEV

#define alSourcePlay TERREATE_AUDIO_NAMESPACE::ALSOURCEPLAY
#define alSourceStop TERREATE_AUDIO_NAMESPACE::ALSOURCESTOP
#define alSourcereWind TERREATE_AUDIO_NAMESPACE::ALSOURCEREWIND
#define alSourcePause TERREATE_AUDIO_NAMESPACE::ALSOURCEPAUSE

#define alSourceQueueBuffers TERREATE_AUDIO_NAMESPACE::ALSOURCEQUEUEBUFFERS
#define alSourceUnqueueBuffers TERREATE_AUDIO_NAMESPACE::ALSOURCEUNQUEUEBUFFERS

#define alGenBuffers TERREATE_AUDIO_NAMESPACE::ALGENBUFFERS
#define alDeleteBuffers TERREATE_AUDIO_NAMESPACE::ALDELETEBUFFERS
#define alIsBuffer TERREATE_AUDIO_NAMESPACE::ALISBUFFER
#define alBufferData TERREATE_AUDIO_NAMESPACE::ALBUFFERDATA

#define alBufferf TERREATE_AUDIO_NAMESPACE::ALBUFFERF
#define alBuffer3f TERREATE_AUDIO_NAMESPACE::ALBUFFER3F
#define alBufferfv TERREATE_AUDIO_NAMESPACE::ALBUFFERFV
#define alBufferi TERREATE_AUDIO_NAMESPACE::ALBUFFERI
#define alBuffer3i TERREATE_AUDIO_NAMESPACE::ALBUFFER3I
#define alBufferiv TERREATE_AUDIO_NAMESPACE::ALBUFFERIV

#define alGetBufferf TERREATE_AUDIO_NAMESPACE::ALGETBUFFERF
#define alGetBuffer3f TERREATE_AUDIO_NAMESPACE::ALGETBUFFER3F
#define alGetBufferfv TERREATE_AUDIO_NAMESPACE::ALGETBUFFERFV
#define alGetBufferi TERREATE_AUDIO_NAMESPACE::ALGETBUFFERI
#define alGetBuffer3i TERREATE_AUDIO_NAMESPACE::ALGETBUFFER3I
#define alGetBufferiv TERREATE_AUDIO_NAMESPACE::ALGETBUFFERIV

#define alDopplerFactor TERREATE_AUDIO_NAMESPACE::ALDOPPLERFACTOR
#define alSpeedofSound TERREATE_AUDIO_NAMESPACE::ALSPEEDOFSOUND
#define alDistanceModel TERREATE_AUDIO_NAMESPACE::ALDISTANCEMODEL

namespace Terreate::Audio::ALhelper {
using namespace Terreate::Types;

Bool Initialize();
void Terminate();

typedef ALCdevice Device;
typedef ALCcontext Context;

typedef Context *(*FPALCCREATECONTEXT)(Device *device, ALCint const *attribs);
typedef Bool (*FPALCMAKECONTEXTCURRENT)(Context *context);
typedef void (*FPALCPROCESSCONTEXT)(Context *context);
typedef void (*FPALCSUSPENDCONTEXT)(Context *context);
typedef void (*FPALCDESTROYCONTEXT)(Context *context);
typedef Context *(*FPALCGETCURRENTCONTEXT)(void);
typedef Device *(*FPALCGETCONTEXTSDEVICE)(Context *context);

typedef Device *(*FPALCOPENDEVICE)(ALCchar const *devicename);
typedef Bool (*FPALCCLOSEDEVICE)(Device *device);

typedef void (*FPALCGETERROR)(Device *device);

typedef ALCboolean (*FPALCISEXTENSIONPRESENT)(Device *device,
                                              ALCchar const *extname);
typedef void *(*FPALCGETPROCADDRESS)(Device *device, ALCchar const *fname);
typedef ALCenum (*FPALCGETENUMVALUE)(Device *device, ALCchar const *ename);

typedef ALCchar const *(*FPALCGETSTRING)(Device *device, ALCenum param);
typedef void (*FPALCGETINTEGERV)(Device *device, ALCenum param, ALCsizei size,
                                 ALCint *data);

typedef Device *(*FPALCCAPTUREOPENDEVICE)(ALCchar const *devicename,
                                          ALCuint freq, ALCenum format,
                                          ALCsizei buffersize);
typedef Bool (*FPALCCAPTURECLOSEDEVICE)(Device *device);
typedef void (*FPALCCAPTURESTART)(Device *device);
typedef void (*FPALCCAPTURESTOP)(Device *device);
typedef void (*FPALCCAPTURESAMPLES)(Device *device, void *buffer,
                                    ALCsizei samples);

typedef void (*FPALENABLE)(ALenum capability);
typedef void (*FPALDISABLE)(ALenum capability);
typedef ALboolean (*FPALISENABLED)(ALenum capability);

typedef ALchar const *(*FPALGETSTRING)(ALenum param);
typedef void (*FPALGETBOOLEANV)(ALenum param, ALchar *data);
typedef void (*FPALGETINTEGERV)(ALenum param, ALint *data);
typedef void (*FPALGETFLOATV)(ALenum param, ALfloat *data);
typedef void (*FPALGETDOUBLEV)(ALenum param, ALdouble *data);
typedef ALboolean (*FPALGETBOOLEAN)(ALenum param);
typedef ALint (*FPALGETINTEGER)(ALenum param);
typedef ALfloat (*FPALGETFLOAT)(ALenum param);
typedef ALdouble (*FPALGETDOUBLE)(ALenum param);

typedef ALenum (*FPALGETERROR)(void);

typedef ALboolean (*FPALISEXTENSIONPRESENT)(ALchar const *extname);
typedef void *(*FPALGETPROCADDRESS)(ALchar const *fname);
typedef ALenum (*FPALGETENUMVALUE)(ALchar const *ename);

typedef void (*FPALLISTENERF)(ALenum param, ALfloat value);
typedef void (*FPALLISTENER3F)(ALenum param, ALfloat value1, ALfloat value2,
                               ALfloat value3);
typedef void (*FPALLISTENERFV)(ALenum param, ALfloat const *values);
typedef void (*FPALLISTENERI)(ALenum param, ALint value);
typedef void (*FPALLISTENER3I)(ALenum param, ALint value1, ALint value2,
                               ALint value3);
typedef void (*FPALLISTENERIV)(ALenum param, ALint const *values);

typedef void (*FPALGETLISTENERF)(ALenum param, ALfloat *value);
typedef void (*FPALGETLISTENER3F)(ALenum param, ALfloat *value1,
                                  ALfloat *value2, ALfloat *value3);
typedef void (*FPALGETLISTENERFV)(ALenum param, ALfloat *values);
typedef void (*FPALGETLISTENERI)(ALenum param, ALint *value);
typedef void (*FPALGETLISTENER3I)(ALenum param, ALint *value1, ALint *value2,
                                  ALint *value3);
typedef void (*FPALGETLISTENERIV)(ALenum param, ALint *values);

typedef void (*FPALGENSOURCES)(Int n, Uint *sources);
typedef void (*FPALDELETESOURCES)(Int n, const Uint *sources);
typedef Bool (*FPALISSOURCE)(Uint source);

typedef void (*FPALSOURCEF)(Uint source, ALenum param, ALfloat value);
typedef void (*FPALSOURCE3F)(Uint source, ALenum param, ALfloat value1,
                             ALfloat value2, ALfloat value3);
typedef void (*FPALSOURCEFV)(Uint source, ALenum param, ALfloat const *values);
typedef void (*FPALSOURCEI)(Uint source, ALenum param, ALint value);
typedef void (*FPALSOURCE3I)(Uint source, ALenum param, ALint value1,
                             ALint value2, ALint value3);
typedef void (*FPALSOURCEIV)(Uint source, ALenum param, ALint const *values);

typedef void (*FPALGETSOURCEF)(Uint source, ALenum param, ALfloat *value);
typedef void (*FPALGETSOURCE3F)(Uint source, ALenum param, ALfloat *value1,
                                ALfloat *value2, ALfloat *value3);
typedef void (*FPALGETSOURCEFV)(Uint source, ALenum param, ALfloat *values);
typedef void (*FPALGETSOURCEI)(Uint source, ALenum param, ALint *value);
typedef void (*FPALGETSOURCE3I)(Uint source, ALenum param, ALint *value1,
                                ALint *value2, ALint *value3);
typedef void (*FPALGETSOURCEIV)(Uint source, ALenum param, ALint *values);

typedef void (*FPALSOURCEPLAYV)(ALsizei ns, ALuint const *ALsids);
typedef void (*FPALSOURCESTOPV)(ALsizei ns, ALuint const *ALsids);
typedef void (*FPALSOURCEREWINDV)(ALsizei ns, ALuint const *ALsids);
typedef void (*FPALSOURCEPAUSEV)(ALsizei ns, ALuint const *ALsids);

typedef void (*FPALSOURCEPLAY)(ALuint source);
typedef void (*FPALSOURCESTOP)(ALuint source);
typedef void (*FPALSOURCEREWIND)(ALuint source);
typedef void (*FPALSOURCEPAUSE)(ALuint source);

typedef void (*FPALSOURCEQUEUEBUFFERS)(ALuint source, ALint nb,
                                       ALuint const *buffers);
typedef void (*FPALSOURCEUNQUEUEBUFFERS)(ALuint source, ALint nb,
                                         ALuint *buffers);

typedef void (*FPALGENBUFFERS)(ALsizei n, ALuint *buffers);
typedef void (*FPALDELETEBUFFERS)(ALsizei n, ALuint const *buffers);
typedef Bool (*FPALISBUFFER)(ALuint bid);
typedef void (*FPALBUFFERDATA)(ALuint buffer, ALenum format, const void *data,
                               ALsizei size, ALsizei freq);

typedef void (*FPALBUFFERF)(ALuint buffer, ALenum param, ALfloat value);
typedef void (*FPALBUFFER3F)(ALuint buffer, ALenum param, ALfloat value1,
                             ALfloat value2, ALfloat value3);
typedef void (*FPALBUFFERFV)(ALuint buffer, ALenum param,
                             ALfloat const *values);
typedef void (*FPALBUFFERI)(ALuint buffer, ALenum param, ALint value);
typedef void (*FPALBUFFER3I)(ALuint buffer, ALenum param, ALint value1,
                             ALint value2, ALint value3);
typedef void (*FPALBUFFERIV)(ALuint buffer, ALenum param, ALint const *values);

typedef void (*FPALGETBUFFERF)(ALuint buffer, ALenum param, ALfloat *value);
typedef void (*FPALGETBUFFER3F)(ALuint buffer, ALenum param, ALfloat *value1,
                                ALfloat *value2, ALfloat *value3);
typedef void (*FPALGETBUFFERFV)(ALuint buffer, ALenum param, ALfloat *values);
typedef void (*FPALGETBUFFERI)(ALuint buffer, ALenum param, ALint *value);
typedef void (*FPALGETBUFFER3I)(ALuint buffer, ALenum param, ALint *value1,
                                ALint *value2, ALint *value3);
typedef void (*FPALGETBUFFERIV)(ALuint buffer, ALenum param, ALint *values);

typedef void (*FPALDOPPLERFACTOR)(ALfloat value);
typedef void (*FPALSPEEDOFSOUND)(ALfloat value);
typedef void (*FPALDISTANCEMODEL)(ALenum value);

extern FPALCCREATECONTEXT ALCCREATECONTEXT;
extern FPALCMAKECONTEXTCURRENT ALCMAKECONTEXTCURRENT;
extern FPALCPROCESSCONTEXT ALCPROCESSCONTEXT;
extern FPALCSUSPENDCONTEXT ALCSUSPENDCONTEXT;
extern FPALCDESTROYCONTEXT ALCDESTROYCONTEXT;
extern FPALCGETCURRENTCONTEXT ALCGETCURRENTCONTEXT;
extern FPALCGETCONTEXTSDEVICE ALCGETCONTEXTSDEVICE;

extern FPALCOPENDEVICE ALCOPENDEVICE;
extern FPALCCLOSEDEVICE ALCCLOSEDEVICE;

extern FPALCGETERROR ALCGETERROR;

extern FPALCISEXTENSIONPRESENT ALCISEXTENSIONPRESENT;
extern FPALCGETPROCADDRESS ALCGETPROCADDRESS;
extern FPALCGETENUMVALUE ALCGETENUMVALUE;

extern FPALCGETSTRING ALCGETSTRING;
extern FPALCGETINTEGERV ALCGETINTEGERV;

extern FPALCCAPTUREOPENDEVICE ALCCAPTUREOPENDEVICE;
extern FPALCCAPTURECLOSEDEVICE ALCCAPTURECLOSEDEVICE;
extern FPALCCAPTURESTART ALCCAPTURESTART;
extern FPALCCAPTURESTOP ALCCAPTURESTOP;
extern FPALCCAPTURESAMPLES ALCCAPTURESAMPLES;

extern FPALENABLE ALENABLE;
extern FPALDISABLE ALDISABLE;
extern FPALISENABLED ALISENABLED;

extern FPALGETSTRING ALGETSTRING;
extern FPALGETBOOLEANV ALGETBOOLEANV;
extern FPALGETINTEGERV ALGETINTEGERV;
extern FPALGETFLOATV ALGETFLOATV;
extern FPALGETDOUBLEV ALGETDOUBLEV;
extern FPALGETBOOLEAN ALGETBOOLEAN;
extern FPALGETINTEGER ALGETINTEGER;
extern FPALGETFLOAT ALGETFLOAT;
extern FPALGETDOUBLE ALGETDOUBLE;

extern FPALGETERROR ALGETERROR;

extern FPALISEXTENSIONPRESENT ALISEXTENSIONPRESENT;
extern FPALGETPROCADDRESS ALGETPROCADDRESS;
extern FPALGETENUMVALUE ALGETENUMVALUE;

extern FPALLISTENERF ALLISTENERF;
extern FPALLISTENER3F ALLISTENER3F;
extern FPALLISTENERFV ALLISTENERFV;
extern FPALLISTENERI ALLISTENERI;
extern FPALLISTENER3I ALLISTENER3I;
extern FPALLISTENERIV ALLISTENERIV;

extern FPALGETLISTENERF ALGETLISTENERF;
extern FPALGETLISTENER3F ALGETLISTENER3F;
extern FPALGETLISTENERFV ALGETLISTENERFV;
extern FPALGETLISTENERI ALGETLISTENERI;
extern FPALGETLISTENER3I ALGETLISTENER3I;
extern FPALGETLISTENERIV ALGETLISTENERIV;

extern FPALGENSOURCES ALGENSOURCES;
extern FPALDELETESOURCES ALDELETESOURCES;
extern FPALISSOURCE ALISSOURCE;

extern FPALSOURCEF ALSOURCEF;
extern FPALSOURCE3F ALSOURCE3F;
extern FPALSOURCEFV ALSOURCEFV;
extern FPALSOURCEI ALSOURCEI;
extern FPALSOURCE3I ALSOURCE3I;
extern FPALSOURCEIV ALSOURCEIV;

extern FPALGETSOURCEF ALGETSOURCEF;
extern FPALGETSOURCE3F ALGETSOURCE3F;
extern FPALGETSOURCEFV ALGETSOURCEFV;
extern FPALGETSOURCEI ALGETSOURCEI;
extern FPALGETSOURCE3I ALGETSOURCE3I;
extern FPALGETSOURCEIV ALGETSOURCEIV;

extern FPALSOURCEPLAYV ALSOURCEPLAYV;
extern FPALSOURCESTOPV ALSOURCESTOPV;
extern FPALSOURCEREWINDV ALSOURCEREWINDV;
extern FPALSOURCEPAUSEV ALSOURCEPAUSEV;

extern FPALSOURCEPLAY ALSOURCEPLAY;
extern FPALSOURCESTOP ALSOURCESTOP;
extern FPALSOURCEREWIND ALSOURCEREWIND;
extern FPALSOURCEPAUSE ALSOURCEPAUSE;

extern FPALSOURCEQUEUEBUFFERS ALSOURCEQUEUEBUFFERS;
extern FPALSOURCEUNQUEUEBUFFERS ALSOURCEUNQUEUEBUFFERS;

extern FPALGENBUFFERS ALGENBUFFERS;
extern FPALDELETEBUFFERS ALDELETEBUFFERS;
extern FPALISBUFFER ALISBUFFER;
extern FPALBUFFERDATA ALBUFFERDATA;

extern FPALBUFFERF ALBUFFERF;
extern FPALBUFFER3F ALBUFFER3F;
extern FPALBUFFERFV ALBUFFERFV;
extern FPALBUFFERI ALBUFFERI;
extern FPALBUFFER3I ALBUFFER3I;
extern FPALBUFFERIV ALBUFFERIV;

extern FPALGETBUFFERF ALGETBUFFERF;
extern FPALGETBUFFER3F ALGETBUFFER3F;
extern FPALGETBUFFERFV ALGETBUFFERFV;
extern FPALGETBUFFERI ALGETBUFFERI;
extern FPALGETBUFFER3I ALGETBUFFER3I;
extern FPALGETBUFFERIV ALGETBUFFERIV;

extern FPALDOPPLERFACTOR ALDOPPLERFACTOR;
extern FPALSPEEDOFSOUND ALSPEEDOFSOUND;
extern FPALDISTANCEMODEL ALDISTANCEMODEL;
} // namespace Terreate::Audio::ALhelper
#endif // __TERREATEAUDIO_ALHELPER_HPP__
