
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:collectLefts [userWallet] [accountWallet] [amount]";
exports.description =  "Allocate votes to accounts";

exports.builder = (yargs) => {
    return yargs.positional('userWallet', {
            type: 'string',
            describe: 'The Account owner of this contract'
          })
          .positional('accountWallet', {
            type: 'string',
            describe: 'The amount of votes minted'
          })
          .positional('amount', {
            type: 'string',
            describe: 'The amount of votes minted'
          });
};

exports.handler =  async (options) =>{
    const {userWallet, accountWallet, amount } = options;
    console.log(`Collecting ${amount} lefts KAI from ${userWallet} to fund ${accountWallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'collectlefts',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            user_wallet: userWallet,
            account_wallet: accountWallet,
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