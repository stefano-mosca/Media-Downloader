#include <formats.h>

const char *const AUDIO_FORMATS[] =
{
    "mp3",
    "m4a",
    "opus",
    "flac",
    "wav",
    "aac"
};

const size_t AUDIO_FORMATS_COUNT = sizeof(AUDIO_FORMATS) / sizeof(AUDIO_FORMATS[0]);

const char *const AUDIO_QUALITIES[] =
{
    "128K",
    "192K",
    "256K",
    "320K",
    "0"
};

const size_t AUDIO_QUALITIES_COUNT = sizeof(AUDIO_QUALITIES) / sizeof(AUDIO_QUALITIES[0]);

const char *const VIDEO_FORMATS[] =
{
    "mp4",
    "mkv",
    "webm"
};

const size_t VIDEO_FORMATS_COUNT = sizeof(VIDEO_FORMATS) / sizeof(VIDEO_FORMATS[0]);

const char *const VIDEO_QUALITIES[] =
{
    "360p",
    "480p",
    "720p",
    "1080p",
    "1440p",
    "2160p",
    "best"
};

const size_t VIDEO_QUALITIES_COUNT = sizeof(VIDEO_QUALITIES) / sizeof(VIDEO_QUALITIES[0]);
