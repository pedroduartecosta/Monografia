# taikai-contracts

All the EOS Taikai Smart Contracts used by Taikai Platform

## Dependencies

* Cmake 3.5
* eoscdt 1.4.1

### Contracts

* [Taikai Token](./contracts/taikai.token/README.md)
* [Taikai Poll](./contracts/taikai.fund/README.md)

## Tasker Commands

* `./scripts/tasker unlock:wallet <name> <password>` - Unlock a Wallet
* `./scripts/tasker deploy <name> <owner_account>` - Deploy a Smart Contract

## Build Contract Instructions

```
git clone git@github.com:hvasconcelos/taikai-contracts.git
git submodule update --init --recursive
./build.sh
```

## How to Run a EOS Node

1. Build the Docker images to launch the Wallet and Node Services

```
docker-compose -f docker/eos-stack.yml build
```

2. Run the Wallet and Node Containers

```
docker-compose -f docker/eos-stack.yml up
```

*Note*: The taikai, helder,luis and mario and taikai.fund taikai.token accounts are created by default during
the initialization