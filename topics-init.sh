#!/bin/bash

# shellcheck disable=SC1004

KAFKA_CREATE_TOPICS=stego-encode-image:1:1,stego-decode-image:1:1

command="kafka-topics --create --topic %s \
--bootstrap-server localhost:9092 \
--replication-factor %d \
--partitions %d\n"

echo ${KAFKA_CREATE_TOPICS} | xargs -d , -I {} echo {} | head -n -1 | awk -F : -v command="$command" '{printf command, $1, $2, $3 }'

