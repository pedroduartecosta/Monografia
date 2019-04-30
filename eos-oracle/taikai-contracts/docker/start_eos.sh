#!/bin/bash
echo "Starting Node Daemon"
nodeos -e -p eosio \
    --plugin eosio::producer_plugin \
    --plugin eosio::chain_api_plugin \
    --plugin eosio::http_plugin \
    --plugin eosio::history_plugin \
    --plugin eosio::history_api_plugin \
    --data-dir /app/eosio/data \
    --config-dir /app/eosio/config \
    --access-control-allow-origin='*' \
    --contracts-console \
    --http-validate-host=false \
    --verbose-http-errors \
    --filter-on='*' & \
sleep 10
/app/eosio/init.sh
wait
