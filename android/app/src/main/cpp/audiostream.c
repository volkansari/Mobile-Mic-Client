#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <android/log.h>
#include <aaudio/AAudio.h>

#include <opus/opus.h>
#include <android/log.h>


#define LOG_TAG "AUDIO_STREAM"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define SAMPLE_RATE 48000
#define CHANNELS 1
#define FRAME_SIZE 960     // 20ms @ 48kHz
#define MAX_PACKET 4000

static int running = 0;
static int udp_sock;
static struct sockaddr_in remote_addr;
static OpusEncoder *encoder;
static AAudioStream *inputStream;

/* ================= AAudio CALLBACK ================= */

aaudio_data_callback_result_t audio_callback(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {

    if (!running) {
        return AAUDIO_CALLBACK_RESULT_STOP;
    }

    int16_t *pcm = (int16_t *) audioData;
    unsigned char opus_data[MAX_PACKET];

    int bytes = opus_encode(
            encoder,
            pcm,
            numFrames,
            opus_data,
            MAX_PACKET
    );

    if (bytes > 0) {
        sendto(
                udp_sock,
                opus_data,
                bytes,
                0,
                (struct sockaddr *) &remote_addr,
                sizeof(remote_addr)
        );
    }

    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

/* ================= START ================= */

void start_stream(const char *ip, int port) {

    if (running) return;
    running = 1;

    /* ---- UDP ---- */
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &remote_addr.sin_addr);

    /* ---- Opus ---- */
    int opus_err;
    encoder = opus_encoder_create(
            SAMPLE_RATE,
            CHANNELS,
            OPUS_APPLICATION_VOIP,
            &opus_err
    );

    if (opus_err != OPUS_OK) {
        LOGE("Opus init failed");
        return;
    }

    opus_encoder_ctl(encoder, OPUS_SET_BITRATE(24000));

    /* ---- AAudio ---- */
    AAudioStreamBuilder *builder;
    AAudio_createStreamBuilder(&builder);

    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);
    AAudioStreamBuilder_setSampleRate(builder, SAMPLE_RATE);
    AAudioStreamBuilder_setChannelCount(builder, CHANNELS);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
    AAudioStreamBuilder_setPerformanceMode(
            builder,
            AAUDIO_PERFORMANCE_MODE_LOW_LATENCY
    );
    AAudioStreamBuilder_setDataCallback(builder, audio_callback, NULL);

    aaudio_result_t result = AAudioStreamBuilder_openStream(builder, &inputStream);
    AAudioStreamBuilder_delete(builder);

    if (result != AAUDIO_OK) {
        LOGE("AAudio open failed: %s", AAudio_convertResultToText(result));
        return;
    }

    AAudioStream_requestStart(inputStream);
    LOGI("Audio stream started");
}

/* ================= STOP ================= */

void stop_stream() {

    if (!running) return;
    running = 0;

    if (inputStream) {
        AAudioStream_requestStop(inputStream);
        AAudioStream_close(inputStream);
        inputStream = NULL;
    }

    if (encoder) {
        opus_encoder_destroy(encoder);
        encoder = NULL;
    }

    close(udp_sock);
    LOGI("Audio stream stopped");
}
