
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:issue [treasury] [wallet] [amount]";
exports.description =  "Creates a poll with an amount of votes available";

exports.builder = (yargs) => {
    return yargs.positional('treasury', {
            type: 'string',
            describe: 'The Issuer'
          })
          .positional('wallet', {
            type: 'string',
            describe: 'The amount of kays to issue'
          }).positional('amount', {
            type: 'string',
            describe: 'The amount of kays to issue'
         });
};

exports.handler =  async (options) =>{
    const {treasury, wallet, amount} = options;
    console.log(`The ${treasury} treasury is issuing ${amount} KAIS to ${wallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'issue',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            treasury,
            wallet,
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