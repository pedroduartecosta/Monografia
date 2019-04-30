#!/bin/bash

WALLET_URL=http://keosd:8888
CLEOS_OPTIONS="--wallet-url=$WALLET_URL"
TEMPDIR=`pwd`

setupTokenAccount(){
    cleos $CLEOS_OPTIONS create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
    return 0;
}

setupAccount(){
    echo "Creating account $1 on Blockchain"
    OWNER_KEY_PUBLIC=`cat $TEMPDIR/$1_owner.key | grep "Public"| awk '{ print $3}'`
    ACTIVE_KEY_PUBLIC=`cat $TEMPDIR/$1_active.key  | grep "Public"| awk '{ print $3}'`
    cleos $CLEOS_OPTIONS create account eosio $1 $OWNER_KEY_PUBLIC $ACTIVE_KEY_PUBLIC -p eosio@active
    return 0;
}

cat taikai.wal | cleos $CLEOS_OPTIONS wallet unlock -n taikai && \
setupTokenAccount && \
setupAccount "hello" && \
setupAccount "taikai" && \
setupAccount "taikai.token" && \
setupAccount "taikai.fund" && \
setupAccount "helder" && \
setupAccount "luis" && \
setupAccount "mario"