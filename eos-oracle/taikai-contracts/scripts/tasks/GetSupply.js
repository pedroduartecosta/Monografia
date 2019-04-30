
const { rpc }  = require('../lib/eosFactory');
const { RpcError } = require('eosjs');

exports.command = "fund:supply";
exports.description =  "Get a list of wallets ";

exports.handler =  async (options) =>{

    try {
      console.log("Getting the list of votes");
      const resp = await rpc.get_table_rows({
        json: true,             // Get the response as json
        code: 'taikai.fund',     // Contract that we target
        scope: 'taikai.fund',    // Account that owns the data
        table: 'treasuries',     // Table name
        limit: 50,              // maximum number of rows that we want to get
      });
      console.log(resp.rows);
    } catch (e) {
      console.log('\nCaught exception: ' + e);
      if (e instanceof RpcError)
        console.log(JSON.stringify(e.json, null, 2));
    }
};