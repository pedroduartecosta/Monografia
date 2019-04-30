#!/bin/bash

WALLET_URL=http://35.195.1.81:8899
NODE_URL=http://35.195.1.81:8888
CLEOS_OPTIONS="--wallet-url=$WALLET_URL --url=$NODE_URL"

cleos $CLEOS_OPTIONS set contract hello ./hello -p hello@active