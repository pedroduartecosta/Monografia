
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:collectAward [challengeWallet] [projectWallet] [amount]";
exports.description =  "Allocate votes to accounts";

exports.builder = (yargs) => {
    return yargs.positional('challengeWallet', {
            type: 'string',
            describe: 'The Account owner of this contract'
          })
          .positional('projectWallet', {
            type: 'string',
            describe: 'The amount of votes minted'
          })
          .positional('amount', {
            type: 'string',
            describe: 'The amount of votes minted'
          });
};

exports.handler =  async (options) =>{
    const {challengeWallet, projectWallet, amount } = options;
    console.log(`Collecting ${amount} KAI from ${projectWallet} to award ${challengeWallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'collectaward',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            ch_wallet: challengeWallet,
            project_wallet: projectWallet,
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