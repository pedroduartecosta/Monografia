
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:fund [wallet] [challengeId] [chWallet] [amount]";
exports.description =  "Creates a poll with an amount of votes available";

exports.builder = (yargs) => {
    return yargs.positional('wallet', {
            type: 'string',
              describe: 'The Issuer'
          }).positional('challengeId', {
            type: 'string',
            describe: 'The Issuer'
          })
          .positional('chWallet', {
            type: 'string',
            describe: 'The amount of kays to fund'
          }).positional('amount', {
            type: 'string',
            describe: 'The amount of kays to fund'
          });
};

exports.handler =  async (options) =>{
    const { wallet, challengeId, chWallet,amount}  = options;
    console.log(`Funding ${amount} KAIS to ${challengeId}:${chWallet} from ${wallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'fund',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            wallet,
            challenge_id: challengeId,
            ch_wallet: chWallet,
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