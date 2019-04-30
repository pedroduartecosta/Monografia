
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:back [challengeWallet] [fromWallet] [toWallet] [quantity]";
exports.description =  "Assign votes to accounts";

exports.builder = (yargs) => {
    return yargs.positional('challengeWallet', {
            type: 'string',
            describe: 'The Account owner of this contract'
          })
          .positional('fromWallet', {
            type: 'string',
            describe: 'The amount of votes minted'
          })
          .positional('toWallet', {
            type: 'string',
            describe: 'The amount of votes minted'
          })
          .positional('quantity', {
            type: 'string',
            describe: 'The amount of votes minted'
          });
};

exports.handler =  async (options) =>{
    const {challengeWallet, fromWallet, toWallet, quantity} = options;
    console.log(`Transferring ${challengeWallet}:${fromWallet} ${quantity} KAIS to ${toWallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'back',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            ch_wallet: challengeWallet,
            user_wallet: fromWallet,
            project_wallet: toWallet,
            amount:quantity,
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