//
// Created by user on 3/13/25.
//
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#include <algorithm>
#include <memory>
#include <string>
#include "wrapper/webrtc_apm_wrapper.h"

class WavDataInfo {
public:
  ~WavDataInfo() {
    if (data) {
      drwav_free(data, NULL);
    }
  }

  bool generateNextFrame(float* dest, size_t frame_count) {
    assert(data != nullptr);
    assert(channels == 1);

    if (currentFrameIndex >= totalPCMFrameCount) {
      return false;
    }

    auto total_sample_count = frame_count * channels;
    std::fill_n(dest, total_sample_count, 0.0f);

    if (currentFrameIndex + frame_count > totalPCMFrameCount) {
      frame_count = totalPCMFrameCount - currentFrameIndex;
    }

    for (int i = 0; i < frame_count; ++i) {
      for (int j = 0; j < channels; ++j) {
        dest[i * channels + j] = data[currentFrameIndex * channels + j];
      }
      ++currentFrameIndex;
    }

    return true;
  }

  unsigned int channels{};
  unsigned int sampleRate{};
  drwav_uint64 totalPCMFrameCount{};
  float* data{nullptr};
  drwav_uint64 currentFrameIndex{0};
};

std::unique_ptr<WavDataInfo> OpenWavAndReadPcm(const std::string& path) {
  unsigned int channels;
  unsigned int sampleRate;
  drwav_uint64 totalPCMFrameCount;
  float* data = drwav_open_file_and_read_pcm_frames_f32(path.c_str(), &channels, &sampleRate, &totalPCMFrameCount, NULL);
  if (data == NULL) {
    printf("Failed to read far wav file: %s\n", path.c_str());
    return nullptr;
  }

  auto wavDataInfo = std::make_unique<WavDataInfo>();
  wavDataInfo->channels = channels;
  wavDataInfo->sampleRate = sampleRate;
  wavDataInfo->totalPCMFrameCount = totalPCMFrameCount;
  wavDataInfo->data = data;

  return wavDataInfo;
}

// using namespace webrtc;
int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    printf("Usage: %s <far_wav_file> <near_wav_file>\n", argv[0]);
    return 1;
  }

  const char* far_wav_file = argv[1];
  const char* near_wav_file = argv[2];

  // load audio data
  auto far_wav_data = OpenWavAndReadPcm(far_wav_file);
  if (far_wav_data == nullptr)
  {
    printf("open far wav file %s failed\n", far_wav_file);
    return 1;
  }

  auto near_wav_data = OpenWavAndReadPcm(near_wav_file);
  if (near_wav_data == nullptr)
  {
    printf("open near wav file %s failed\n", near_wav_file);
    return 1;
  }

  assert(far_wav_data->sampleRate == near_wav_data->sampleRate);
  if (far_wav_data->channels != 1 || near_wav_data->channels != 1)
  {
    printf("only support mono audio\n");
    return 1;
  }

  // config output wave
  drwav_data_format format;
  format.container = drwav_container_riff;
  format.format = DR_WAVE_FORMAT_IEEE_FLOAT;
  format.channels = far_wav_data->channels;
  format.sampleRate = far_wav_data->sampleRate;
  format.bitsPerSample = 32; // float
  drwav wav;
  const char* output_path = "aec3_demo_output.wav";
  auto ok = drwav_init_file_write(&wav, output_path, &format, NULL);
  if (!ok) {
    printf("open output wave file failed\n");
    return -1;
  }




  // create APM
  auto* apm = webrtc_apm_create();
  APMConfig config;
  config.echo_canceller.enabled = true;
  webrtc_apm_apply_config(apm, &config);

  // prepare for process
  webrtc_apm_prepare(apm, far_wav_data->sampleRate, far_wav_data->channels);

  // processing and write to output wave
  const int samples_per_frame = far_wav_data->sampleRate / 100;
  std::vector<float> far_frame(samples_per_frame);
  std::vector<float> near_frame(samples_per_frame);
  for (;;) {
    if (!far_wav_data->generateNextFrame(far_frame.data(), samples_per_frame)) {
      break;
    }

    if (!near_wav_data->generateNextFrame(near_frame.data(), samples_per_frame)) {
      break;
    }

    const float* input_src[1] = {far_frame.data()};
    float* input_dest[1] = {far_frame.data()};
    webrtc_apm_process_reverse_stream(apm, input_src, input_dest);

    const float* output_src[1] = {near_frame.data()};
    float* output_dest[1] = {near_frame.data()};
    webrtc_apm_process_stream(apm, output_src, output_dest);

    // write output to wav
    drwav_uint64 samples_written = drwav_write_pcm_frames(&wav, samples_per_frame, near_frame.data());
    if (samples_written <= 0) {
      drwav_uninit(&wav);
      return -1;
    }
  }

  webrtc_apm_destroy(apm);

  printf("aec processing done, result write to %s\n", output_path);
  drwav_uninit(&wav);

  return 0;
}