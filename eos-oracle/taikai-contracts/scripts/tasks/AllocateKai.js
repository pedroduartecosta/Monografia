
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:allocate [challengeWallet] [userWallet] [quantity]";
exports.description =  "Allocate votes to accounts";

exports.builder = (yargs) => {
    return yargs.positional('challengeWallet', {
            type: 'string',
            describe: 'The Account owner of this contract'
          })
          .positional('userWallet', {
            type: 'string',
            describe: 'The amount of votes minted'
          })
          .positional('quantity', {
            type: 'string',
            describe: 'The amount of votes minted'
          });
};

exports.handler =  async (options) =>{
    const {challengeWallet, userWallet, quantity } = options;
    console.log(`Allocating ${quantity} to ${userWallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'allocate',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            ch_wallet: challengeWallet,
            user_wallet: userWallet,
            amount: quantity,
          },
        }]
      }, {
        blocksBehind: 3,
        expireSeconds: 30,
      });
        console.log("result"+JSON.stringify(result));
    } catch (e) {
      console.log('\nCaught exception: ' + e);
      if (e instanceof RpcError)
        console.log(JSON.stringify(e.json, null, 2));
    }
};