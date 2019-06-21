# Implementing a Blockchain Oracle on Ethereum

TLDR: In this post, I'll explain how to build an ethereum self-hosted blockchain oracle. A service to query APIs upon request by smart-contracts. Additionally this service, allows for having multiple competing parties deploying the same oracle to have the same say in the result.

Smart Contracts in Ethereum can power a wide range of applications, however, and due to the nature of the blockchain, smart contracts lack a essential feature: *Internet Connectivity*.

### Why? you may ask
The Ethereum blockchain is designed to be entirely deterministic, meaning that if someone downloads the whole network history and replays it they should always end up with the same state. Determinism is necessary so that nodes can come to a consensus.

However, the internet is not deterministic, a smart contract querying an API at a certain point in time, cannot be guaranteed that later querying the same API will get the same result. APIs and data on the web change. Therefore, by nature smart contracts lack connectivity.

### So Oracles right?
The name oracle comes from the fact that oracles, historically speaking were sources of the truth. And that's what we need.

Oracles are services that insert data on the blockchain to be used by smart contract. By adding a transaction with the required information to the blockchain the smart contract can ran and always obtain the same information since it is retrieving it from the blockchain.

### Architecture

[Add image here]
