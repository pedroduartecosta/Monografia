# Taikai Smart Contract

### Tables

* issuer
* cwallets
* pwallets
* uwallets

### Actions

### taikai.fund::create
Issue new KAIS tokens

### Arguments

* issuer
* supply

### taikai.fund::issue
Transfer KAIS to challenge wallet

### Arguments

* challenge_id
* ch_wallet
* amount

### taikai.fund::assign
Transfer KAIS to challenge user wallet

### Arguments

* ch_wallet
* user_wallet
* amount


### taikai.fund::transfer
Transfer KAIS on challenge wallets between users

### Arguments

* ch_wallet
* from_wallet
* to_wallet
* amount

### taikai.fund::back
Transfer KAIS to project wallet

### Arguments

* ch_wallet
* user_wallet
* project_wallet
* amount


### taikai.fund::close
Close all the challenge wallets

### Arguments

* ch_wallet




