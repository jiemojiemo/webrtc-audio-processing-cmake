//
// Created by user on 3/14/25.
//
#include "webrtc_apm_wrapper.h"

#include <modules/audio_processing/include/audio_processing.h>

class WebRTCApm {
public:
  ~WebRTCApm() {
    delete apm;
  }
  webrtc::AudioProcessing* apm{nullptr};
  webrtc::StreamConfig input_stream_config;
  webrtc::StreamConfig output_stream_config;
};

void* webrtc_apm_create()
{
  auto *handle = new WebRTCApm();
  handle->apm = webrtc::AudioProcessingBuilder().Create();
  return handle;
}

void webrtc_apm_destroy(void* apm) {
  auto* handle = static_cast<WebRTCApm*>(apm);
  if (handle) {
    delete handle;
  }
}

void webrtc_apm_prepare(void* apm, int sample_rate,int channels) {
  auto* handle = static_cast<WebRTCApm*>(apm);
  if (!handle) {
    return;
  }

  handle->input_stream_config = webrtc::StreamConfig(sample_rate, channels);
  handle->output_stream_config = webrtc::StreamConfig(sample_rate, channels);
}

void webrtc_apm_apply_config(void *apm, const APMConfig *config) {
  auto* handle = static_cast<WebRTCApm*>(apm);
  if (!handle) {
    return;
  }

  if (!config) {
    return;
  }

  auto cfg = webrtc::AudioProcessing::Config();
  cfg.echo_canceller.enabled = config->echo_canceller.enabled;

  handle->apm->ApplyConfig(cfg);
}

void webrtc_apm_process_reverse_stream(void* apm, const float* const* src, float* const* dest) {
  auto* handle = static_cast<WebRTCApm*>(apm);
  if (!handle) {
    return;
  }

  handle->apm->ProcessReverseStream(src, handle->input_stream_config, handle->output_stream_config, dest);
}

void webrtc_apm_process_stream(void* apm, const float* const* src, float* const* dest) {
  auto* handle = static_cast<WebRTCApm*>(apm);
  if (!handle) {
    return;
  }

  handle->apm->ProcessStream(src, handle->input_stream_config, handle->output_stream_config, dest);
}
