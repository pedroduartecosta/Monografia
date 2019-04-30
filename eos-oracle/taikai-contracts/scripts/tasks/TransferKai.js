
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:transfer [challengeWallet] [from] [to] [amount]";
exports.description =  "Creates a poll with an amount of votes available";

exports.builder = (yargs) => {
    return yargs.positional('challengeWallet', {
            type: 'string',
            describe: 'The Issuer'
          })
          .positional('from', {
            type: 'string',
            describe: 'The amount of kays to transfer'
          }).positional('to', {
            type: 'string',
            describe: 'The amount of kays to transfer'
          }).positional('amount', {
            type: 'string',
            describe: 'The amount of kays to transfer'
          });
};

exports.handler =  async (options) =>{
    const { challengeWallet, from,to,amount}  = options;
    console.log(`Transfering ${amount} KAIS from ${from} to ${to}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'transfer',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            ch_wallet: challengeWallet,
            from_wallet: from,
            to_wallet: to,
            amount,
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