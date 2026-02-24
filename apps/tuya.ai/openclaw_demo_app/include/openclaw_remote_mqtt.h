/**
 * @file openclaw_remote_mqtt.h
 * @brief Openclaw remote MQTT client: send user speech text to a remote server
 *        and receive server responses.
 *
 * The device publishes transcribed user speech (ASR text) to the remote MQTT
 * broker and subscribes to the server response topic.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */

#ifndef __OPENCLAW_REMOTE_MQTT_H__
#define __OPENCLAW_REMOTE_MQTT_H__

#include "tuya_cloud_types.h"
#include <stddef.h>
#include <stdint.h>

/** MQTT QoS 1 (at least once) for openclaw_remote_mqtt_send_user_text when delivery matters. */
#define OPENCLAW_REMOTE_MQTT_QOS_AT_LEAST_ONCE 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback invoked when a message is received on the server response topic.
 *
 * @param payload Message payload (may be NULL if length is 0).
 * @param length Payload length in bytes.
 * @param userdata User context passed to openclaw_remote_mqtt_register_response_callback().
 */
typedef void (*openclaw_remote_mqtt_response_cb_t)(const uint8_t *payload, size_t length, void *userdata);

/**
 * @brief Send user speech text (e.g. ASR result) to the remote MQTT server.
 *
 * Publishes the given text to the device user-speech topic so the remote
 * server can process it (e.g. for openclaw control or AI response).
 *
 * @param text UTF-8 text (e.g. transcribed speech). Must not be NULL.
 * @param text_len Length of text in bytes.
 * @param qos MQTT QoS (0, 1, or 2). Use 1 for at-least-once delivery.
 * @return Message ID on success, 0 on failure.
 */
uint16_t openclaw_remote_mqtt_send_user_text(const uint8_t *text, size_t text_len, uint8_t qos);

/**
 * @brief Register a callback for messages received on the server response topic.
 *
 * @param callback Called when the remote server publishes a response. Must not be NULL.
 * @param userdata Passed to callback on each invocation.
 * @return 0 on success, -1 on error (e.g. callback is NULL).
 */
int openclaw_remote_mqtt_register_response_callback(openclaw_remote_mqtt_response_cb_t callback, void *userdata);

/**
 * @brief Unregister the server response callback.
 */
void openclaw_remote_mqtt_unregister_response_callback(void);

/**
 * @brief Initialize and connect the remote MQTT client.
 *
 * Call once after the device has network connectivity (e.g. after Tuya MQTT
 * is connected). The client will subscribe to the server response topic
 * upon connection.
 */
void openclaw_remote_mqtt_init(void);

/**
 * @brief Process incoming MQTT packets and keep the connection alive.
 *
 * Call periodically from the main loop (e.g. after tuya_iot_yield).
 */
void openclaw_remote_mqtt_yield(void);

#ifdef __cplusplus
}
#endif

#endif /* __OPENCLAW_REMOTE_MQTT_H__ */
