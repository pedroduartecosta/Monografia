# Taikai Token Smart Token

## Deploy Instructions

1. Please first create an account for the token contract

```
cleos create account eosio taikai.token <OWNER_KEY> <ACTIVE_KEY>
```

2. Deploy the token to the Single Net Node

```
WALLET_URL=http://35.240.55.207:8899
NODE_URL=http://35.240.55.207:8888
CLEOS_OPTIONS="--wallet-url=$WALLET_URL --url=$NODE_URL"
cleos $CLEOS_OPTIONS wallet unlock -n taikai
cleos $CLEOS_OPTIONS set contract taikai.token ./build/contracts/taikai.token/ -p taikai.token@active
```


3. Create the Taikai Token

```
 cleos $CLEOS_OPTIONS push action taikai.token create '[ "taikai", "1000000000.0000 TAK"]' -p taikai.token@active
 ```

4. Issue Tokens from the original supply

```
cleos $CLEOS_OPTIONS push action taikai.token issue '[ "helder", "100.0000 TAK", "memo" ]' -p taikai@active
```

5. Transfer Tokens  from one account to other

```
cleos $CLEOS_OPTIONS push action taikai.token transfer '[ "helder", "luis", "25.0000 TAK", "m" ]' -p helder@active
```

6. Get The User Balance

```
cleos $CLEOS_OPTIONS get currency balance taikai.token helder TAK
```