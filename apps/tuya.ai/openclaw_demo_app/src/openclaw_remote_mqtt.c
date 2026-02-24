/**
 * @file openclaw_remote_mqtt.c
 * @brief Openclaw remote MQTT client implementation.
 *
 * Connects to a remote MQTT broker to send user speech text (from device ASR)
 * and receive server responses. Topic names are verbose for clarity.
 *
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */

#include "openclaw_remote_mqtt.h"
#include "mqtt_client_interface.h"
#include "tal_api.h"
#include "tkl_output.h"
#include <string.h>

/***********************************************************
*********************** Topic names ************************
***********************************************************/
/** Topic where the device publishes user speech text (ASR) for the remote server. */
#define OPENCLAW_TOPIC_DEVICE_USER_SPEECH_TEXT "openclaw/device/user_speech_text"

/** Topic where the device subscribes to receive responses from the remote server. */
#define OPENCLAW_TOPIC_SERVER_RESPONSE "openclaw/server/response"

/**
 * Remote MQTT broker host.
 *
 * NOTE: Please update the IP address below to match your actual MQTT broker.
 */
#define OPENCLAW_REMOTE_MQTT_BROKER_HOST "192.168.100.132"

/** Remote MQTT broker port. */
#define OPENCLAW_REMOTE_MQTT_BROKER_PORT 1883

#ifndef MQTT_KEEPALIVE_INTERVALIN
#define MQTT_KEEPALIVE_INTERVALIN (120)
#endif
#ifndef MATOP_TIMEOUT_MS_DEFAULT
#define MATOP_TIMEOUT_MS_DEFAULT (8000U)
#endif

/***********************************************************
*********************** State ******************************
***********************************************************/
static void                              *g_mqtt_ctx;
static openclaw_remote_mqtt_response_cb_t g_response_cb;
static void                              *g_response_userdata;
static uint16_t                           g_subscribe_msgid;
static bool                               g_connected;

/***********************************************************
*********************** Callbacks **************************
***********************************************************/
static void on_connected(void *client, void *userdata)
{
    (void)userdata;
    PR_INFO("Openclaw remote MQTT connected, subscribing to %s", OPENCLAW_TOPIC_SERVER_RESPONSE);
    g_connected = true;

    uint16_t msgid = mqtt_client_subscribe(client, OPENCLAW_TOPIC_SERVER_RESPONSE, MQTT_QOS_1);
    if (msgid > 0) {
        g_subscribe_msgid = msgid;
        PR_INFO("Subscribed to server response topic, msgid: %d", msgid);
    } else {
        PR_ERR("Subscribe to %s failed", OPENCLAW_TOPIC_SERVER_RESPONSE);
    }
}

static void on_disconnected(void *client, void *userdata)
{
    (void)client;
    (void)userdata;
    PR_INFO("Openclaw remote MQTT disconnected");
    g_connected = false;
}

static void on_message(void *client, uint16_t msgid, const mqtt_client_message_t *msg, void *userdata)
{
    (void)client;
    (void)msgid;
    (void)userdata;
    PR_DEBUG("Openclaw remote MQTT message topic: %s, len: %d", msg->topic, (int)msg->length);

    if (msg->topic == NULL || strcmp(msg->topic, OPENCLAW_TOPIC_SERVER_RESPONSE) != 0) {
        return;
    }

    PR_INFO("Received server response, length: %zu", msg->length);
    if (msg->payload != NULL && msg->length > 0) {
        PR_INFO("Server response payload: %.*s", (int)msg->length, (const char *)msg->payload);
    }

    if (g_response_cb != NULL) {
        g_response_cb(msg->payload, msg->length, g_response_userdata);
    } else {
        PR_INFO("No server response callback registered, message ignored");
    }
}

static void on_subscribed(void *client, uint16_t msgid, void *userdata)
{
    (void)client;
    (void)userdata;
    PR_DEBUG("Openclaw remote MQTT subscribe ack msgid: %d", msgid);
    if (msgid == g_subscribe_msgid && g_response_cb != NULL) {
        PR_INFO("Server response topic subscribed successfully");
    }
}

static void on_published(void *client, uint16_t msgid, void *userdata)
{
    (void)client;
    (void)msgid;
    (void)userdata;
    PR_DEBUG("Openclaw remote MQTT publish ack msgid: %d", msgid);
}

/***********************************************************
*********************** Public API *************************
***********************************************************/
uint16_t openclaw_remote_mqtt_send_user_text(const uint8_t *text, size_t text_len, uint8_t qos)
{
    if (g_mqtt_ctx == NULL) {
        PR_ERR("Openclaw remote MQTT not initialized");
        return 0;
    }
    if (!g_connected) {
        PR_ERR("Openclaw remote MQTT not connected");
        return 0;
    }
    if (text == NULL || text_len == 0) {
        PR_ERR("openclaw_remote_mqtt_send_user_text: text is NULL or length is 0");
        return 0;
    }

    uint16_t msgid = mqtt_client_publish(g_mqtt_ctx, OPENCLAW_TOPIC_DEVICE_USER_SPEECH_TEXT, text, text_len, qos);
    if (msgid > 0) {
        PR_DEBUG("Sent user speech text to %s, msgid: %d, len: %zu", OPENCLAW_TOPIC_DEVICE_USER_SPEECH_TEXT, msgid,
                 text_len);
    } else {
        PR_ERR("Publish to %s failed", OPENCLAW_TOPIC_DEVICE_USER_SPEECH_TEXT);
    }
    return msgid;
}

int openclaw_remote_mqtt_register_response_callback(openclaw_remote_mqtt_response_cb_t callback, void *userdata)
{
    if (callback == NULL) {
        PR_ERR("openclaw_remote_mqtt_register_response_callback: callback is NULL");
        return -1;
    }
    g_response_cb       = callback;
    g_response_userdata = userdata;
    PR_DEBUG("Openclaw remote MQTT response callback registered");
    return 0;
}

void openclaw_remote_mqtt_unregister_response_callback(void)
{
    g_response_cb       = NULL;
    g_response_userdata = NULL;
    PR_DEBUG("Openclaw remote MQTT response callback unregistered");
}

void openclaw_remote_mqtt_init(void)
{
    PR_DEBUG("Openclaw remote MQTT client starting");

    g_mqtt_ctx = mqtt_client_new();
    if (g_mqtt_ctx == NULL) {
        PR_ERR("Openclaw remote MQTT allocation failed");
        return;
    }

    const mqtt_client_config_t config = {
        .cacert          = NULL,
        .cacert_len      = 0,
        .host            = OPENCLAW_REMOTE_MQTT_BROKER_HOST,
        .port            = OPENCLAW_REMOTE_MQTT_BROKER_PORT,
        .keepalive       = MQTT_KEEPALIVE_INTERVALIN,
        .timeout_ms      = MATOP_TIMEOUT_MS_DEFAULT,
        .clientid        = "tuya-open-sdk-for-device-01",
        .username        = "emqx",
        .password        = "public",
        .on_connected    = on_connected,
        .on_disconnected = on_disconnected,
        .on_message      = on_message,
        .on_subscribed   = on_subscribed,
        .on_published    = on_published,
        .userdata        = NULL,
    };

    mqtt_client_status_t status = mqtt_client_init(g_mqtt_ctx, &config);
    if (status != MQTT_STATUS_SUCCESS) {
        PR_ERR("Openclaw remote MQTT init failed: %d", status);
        mqtt_client_free(g_mqtt_ctx);
        g_mqtt_ctx = NULL;
        return;
    }

    status = mqtt_client_connect(g_mqtt_ctx);
    if (status == MQTT_STATUS_NOT_AUTHORIZED) {
        PR_ERR("Openclaw remote MQTT connect failed: %d", status);
        mqtt_client_deinit(g_mqtt_ctx);
        mqtt_client_free(g_mqtt_ctx);
        g_mqtt_ctx = NULL;
        return;
    }

    PR_INFO("Openclaw remote MQTT client connect started");
}

void openclaw_remote_mqtt_yield(void)
{
    if (g_mqtt_ctx != NULL) {
        mqtt_client_yield(g_mqtt_ctx);
    }
}
