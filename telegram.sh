#!/bin/bash

STAGE_TYPE=$1
BOT_TOKEN="TOKEN"
CHAT_ID="ID_USER"

if [ "$CI_JOB_STATUS" == "success" ]; then
    MESSAGE="Stage $STAGE_TYPE $CI_JOB_NAME completed successfully%0AURL: $CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID"
else
    MESSAGE="Stage $STAGE_TYPE $CI_JOB_NAME failed%0AURL: $CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID"
fi

curl -X POST "https://api.telegram.org/bot$BOT_TOKEN/sendMessage" -d chat_id=$CHAT_ID -d text="$MESSAGE"
