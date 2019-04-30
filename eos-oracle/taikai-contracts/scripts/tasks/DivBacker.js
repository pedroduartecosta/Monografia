
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:divBacker [challengeWallet] [userWallet] [amount]";
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
          .positional('amount', {
            type: 'string',
            describe: 'The amount of votes minted'
          });
};

exports.handler =  async (options) =>{
    const {challengeWallet, userWallet, amount } = options;
    console.log(`Distributing ${amount} KAI from ${challengeWallet} to ${userWallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'divbacker',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            ch_wallet: challengeWallet,
            user_wallet: userWallet,
            amount: amount
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