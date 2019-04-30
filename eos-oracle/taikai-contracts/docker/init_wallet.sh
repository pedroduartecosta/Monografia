#!/bin/bash

WALLET_URL=http://localhost:8888
TEMPDIR=`pwd`

launchKeosd() {
    keosd --http-server-address=localhost:8888 -d /app/eosio/data --http-validate-host=false &
}

setupWallet(){
    echo "Setting up the wallet"
    cleos wallet create -f taikai.wal -n taikai
    echo "The password for wallet is $(cat taikai.wal)"
}

setupEOSAccount(){
    echo "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3" | \
    cleos wallet import -n taikai
}
setupAccount(){
    echo "Setting up account $1 on wallet"
    cleos create key --file $TEMPDIR/$1_owner.key
    cleos create key --file $TEMPDIR/$1_active.key
    OWNER_KEY_PRIVATE=`cat $TEMPDIR/$1_owner.key | grep "Private"| awk '{ print $3}'`
    ACTIVE_KEY_PRIVATE=`cat $TEMPDIR/$1_active.key  | grep "Private"| awk '{ print $3}'`
    OWNER_KEY_PUBLIC=`cat $TEMPDIR/$1_owner.key | grep "Public"| awk '{ print $3}'`
    ACTIVE_KEY_PUBLIC=`cat $TEMPDIR/$1_active.key  | grep "Public"| awk '{ print $3}'`
    echo "Public Owner Key for $1 is $OWNER_KEY_PUBLIC"
    echo "Public Active Key for $1 is $ACTIVE_KEY_PUBLIC"
    echo $OWNER_KEY_PRIVATE | cleos wallet import -n taikai
    echo $ACTIVE_KEY_PRIVATE | cleos wallet import -n taikai
}

launchKeosd
setupWallet
setupEOSAccount
setupAccount "hello"
setupAccount "taikai"
setupAccount "taikai.token"
setupAccount "taikai.fund"
setupAccount "helder"
setupAccount "luis"
setupAccount "mario"
pkill keosd
cp ~/eosio-wallet/taikai.wallet data/