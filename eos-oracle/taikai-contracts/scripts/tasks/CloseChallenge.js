
const { api } = require('../lib/eosFactory');
const { RpcError } = require('eosjs');
exports.command = "fund:close [chWallet]";
exports.description =  "Assign votes to accounts";

exports.builder = (yargs) => {
    return yargs.positional('chWallet', {
            type: 'string',
            describe: 'The Account owner of this contract'
          });
};

exports.handler =  async (options) =>{
    const { chWallet } = options;
    console.log(`Close Challenge  Wallets ${chWallet}`);
    try {
      const result = await api.transact({
        actions: [{
          account: 'taikai.fund',
          name: 'close',
          authorization: [{
            actor: 'taikai.fund',
            permission: 'active',
          }],
          data: {
            ch_wallet: chWallet,
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