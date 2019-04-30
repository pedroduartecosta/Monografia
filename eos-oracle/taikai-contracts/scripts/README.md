## Tools To deploy

### Tasker
A tool to deploy the Taikai contract and call the actions  required to exercise taikai Smart Contract

#### Deploy a contract

```
./scripts/tasker.js deploy taikai.fund taikai.fund
```

#### Get the Kais Supply

```
/scripts/tasker.js fund:supply
```

#### Mint Kais

```
./scripts/tasker.js fund::mint taikai.fund 10000000
```

#### Issue Kais

```
./scripts/tasker.js fund:issue test KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a35a 1000000 “test”
```

#### Get the challenge wallets

```
 ./scripts/tasker.js fund:wallet:challenge:list
 ```

#### Allocate Kais

```
./scripts/tasker.js fund:allocate  KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a35a KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a359 10000 test
```

#### Transfer Kais to Other user

```
./scripts/tasker.js fund:transfer KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a35a KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a359 KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a360 200 test
```

#### Back a project

```
 ./scripts/tasker.js fund:back KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a35a KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a359 KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a369 10 test
```

#### Get the project Wallet list

```
./scripts/tasker.js fund:wallet:project:list
```

#### Close Challenge

```
./scripts/tasker.js fund:close KAI017d62c4bd10c6462f453dd079b1ef9add826357eb358571db795074554890a35a
```