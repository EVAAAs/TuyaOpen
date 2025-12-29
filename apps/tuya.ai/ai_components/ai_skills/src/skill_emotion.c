/**
 * @file skill_emotion.c
 * @brief skill_emotion module is used to 
 * @version 0.1
 * @copyright Copyright (c) 2021-2025 Tuya Inc. All Rights Reserved.
 */
#include "tuya_cloud_types.h"
#include "mix_method.h"

#include "tal_api.h"
#include "cJSON.h"

#include "ai_user_event.h"
#include "skill_emotion.h"

/***********************************************************
************************macro define************************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/


/***********************************************************
***********************function define**********************
***********************************************************/
OPERATE_RET ai_agent_parse_emo(cJSON *json, AI_AGENT_EMO_T **emo)
{
    cJSON *emotion, *text;
    int emo_cnt = 0;
    AI_AGENT_EMO_T *emo_ptr;

    json = cJSON_GetObjectItem(json, "skillContent");
    if (!json) {
        PR_ERR("skill content is NULL");
        return OPRT_OK;
    }

    emotion = cJSON_GetObjectItem(json, "emotion");
    text = cJSON_GetObjectItem(json, "text");
    emo_cnt = cJSON_GetArraySize(emotion);
    if (emo_cnt == 0) {
        PR_ERR("emo array is empty");
        return OPRT_CJSON_GET_ERR;
    }

    emo_ptr = tal_malloc(sizeof(AI_AGENT_EMO_T));
    if (emo_ptr == NULL) {
        PR_ERR("malloc emo fail.");
        return OPRT_MALLOC_FAILED;
    }

    memset(emo_ptr, 0, sizeof(AI_AGENT_EMO_T));

    emo_ptr->emo_cnt = emo_cnt;
    emo_ptr->emotion = tal_malloc(sizeof(char *) * emo_cnt);
    if (emo_ptr->emotion == NULL) {
        PR_ERR("malloc emo fail.");
        tal_free(emo_ptr);
        return OPRT_MALLOC_FAILED;
    }

    emo_ptr->text = tal_malloc(sizeof(char *) * emo_cnt);
    if (emo_ptr->text == NULL) {
        PR_ERR("malloc emo fail.");
        tal_free(emo_ptr->emotion);
        tal_free(emo_ptr);
        return OPRT_MALLOC_FAILED;
    }

    int i = 0;
    for (i = 0; i < emo_cnt; i++) {
        cJSON *emo = cJSON_GetArrayItem(emotion, i);
        emo_ptr->emotion[i] = mm_strdup(emo->valuestring);
    }

    for (i = 0; i < emo_cnt; i++) {
        cJSON *txt = cJSON_GetArrayItem(text, i);
        if (!txt)
            emo_ptr->text[i] = mm_strdup("");
        else
            emo_ptr->text[i] = mm_strdup(txt->valuestring);
    }

    *emo = emo_ptr;

    return OPRT_OK;
}

OPERATE_RET ai_agent_play_emo(AI_AGENT_EMO_T *emo)
{
    // send data to register cb
    ai_user_event_notify(AI_USER_EVT_EMOTION, emo);

    return OPRT_OK;    
}

void ai_agent_parse_emo_free(AI_AGENT_EMO_T *emo)
{
    if (!emo)
        return;

    if (emo->emotion) {
        int i = 0;
        for (i = 0; i < emo->emo_cnt; i++) {
            tal_free(emo->emotion[i]);
        }
        tal_free(emo->emotion);
    }

    if (emo->text) {
        int i = 0;
        for (i = 0; i < emo->emo_cnt; i++) {
            tal_free(emo->text[i]);
        }
        tal_free(emo->text);
    }

    tal_free(emo);
}