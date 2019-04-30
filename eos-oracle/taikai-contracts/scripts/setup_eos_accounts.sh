#!/bin/bash

WALLET_URL=http://127.0.0.1:8900
NODE_URL=http://127.0.0.1:8888
CLEOS_OPTIONS="--wallet-url=$WALLET_URL --url=$NODE_URL"
#CLEOS_OPTIONS=""
TEMPDIR=./tmp

setupWallet(){
    echo "Setting up the wallet"
    echo "cleos $CLEOS_OPTIONS wallet create -f $TEMPDIR/taikai.wal -n taikai"
    cleos $CLEOS_OPTIONS wallet create -f $TEMPDIR/taikai.wal -n taikai
}

setupEOSAccount(){
    echo "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3" | \
    cleos $CLEOS_OPTIONS wallet import -n taikai
}
setupAccount(){
    cleos $CLEOS_OPTIONS create key --file $TEMPDIR/$1_owner.key
    cleos $CLEOS_OPTIONS create key --file $TEMPDIR/$1_active.key
    OWNER_KEY_PRIVATE=`cat $TEMPDIR/$1_owner.key | grep "Private"| awk '{ print $3}'`
    ACTIVE_KEY_PRIVATE=`cat $TEMPDIR/$1_active.key  | grep "Private"| awk '{ print $3}'`
    OWNER_KEY_PUBLIC=`cat $TEMPDIR/$1_owner.key | grep "Public"| awk '{ print $3}'`
    ACTIVE_KEY_PUBLIC=`cat $TEMPDIR/$1_active.key  | grep "Public"| awk '{ print $3}'`
    echo "Public Owner Key for $1 is $OWNER_KEY_PUBLIC"
    echo "Public Active Key for $1 is $ACTIVE_KEY_PUBLIC"
    echo $OWNER_KEY_PRIVATE | cleos $CLEOS_OPTIONS wallet import -n taikai
    echo $ACTIVE_KEY_PRIVATE | cleos $CLEOS_OPTIONS wallet import -n taikai
    cleos $CLEOS_OPTIONS create account eosio $1 $OWNER_KEY_PUBLIC $ACTIVE_KEY_PUBLIC -p eosio@active
    if [ "$1" = "taikai.fund" ]
    then
        echo -e "WALLET_URL=http://127.0.0.1:8900\r\nNODE_URL=http://127.0.0.1:8888\r\nTAKAI_FUND_PRIVATE_KEY=$ACTIVE_KEY_PRIVATE" >> ".env"
    fi
    if [ "$1" = "taikai.token" ]
    then
        echo -e "TAKAI_TOKEN_PRIVATE_KEY=$ACTIVE_KEY_PRIVATE" >> ".env"
    fi
}

mkdir -p ./tmp
setupWallet
setupEOSAccount
setupAccount "hello"
setupAccount "taikai.fund"
setupAccount "taikai.token"
setupAccount "helder"
