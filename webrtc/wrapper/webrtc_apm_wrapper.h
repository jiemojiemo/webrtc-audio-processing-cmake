//
// Created by user on 3/14/25.
//

#ifndef WEBRTC_APM_WRAPPER_H
#define WEBRTC_APM_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct APMConfigEchoCanceller {
  bool enabled = false;
  bool mobile_mode = false;
  bool export_linear_aec_output = false;
  // Enforce the highpass filter to be on (has no effect for the mobile
  // mode).
  bool enforce_high_pass_filtering = true;
}APMConfigEchoCanceller;

typedef struct APMConfig {
  APMConfigEchoCanceller echo_canceller;
}APMConfig;

void *webrtc_apm_create();
void webrtc_apm_destroy(void *apm);

void webrtc_apm_apply_config(void *apm, const APMConfig *config);
void webrtc_apm_prepare(void *apm, int sample_rate, int channels);
void webrtc_apm_process_reverse_stream(void *apm, const float *const *src,
                                       float *const *dest);
void webrtc_apm_process_stream(void *apm, const float *const *src,
                               float *const *dest);

#ifdef __cplusplus
}
#endif

#endif // WEBRTC_APM_WRAPPER_H
