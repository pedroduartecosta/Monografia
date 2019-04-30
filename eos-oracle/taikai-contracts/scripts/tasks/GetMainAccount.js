const { rpc }  = require('../lib/eosFactory');
exports.command = "fund:issuer";
exports.description =  "Get a list of open polls";
const { RpcError } = require('eosjs');

exports.handler =  async (options) =>{

    try {
      console.log("Getting the list of poll channels");
      const resp = await rpc.get_table_rows({
        json: true,             // Get the response as json
        code: 'taikai.poll',     // Contract that we target
        scope: 'taikai.poll',    // Account that owns the data
        table: 'issuer',  // Table name
        limit: 50,              // maximum number of rows that we want to get
      });
      console.log(resp.rows);
    } catch (e) {
      console.log('\nCaught exception: ' + e);
      if (e instanceof RpcError)
        console.log(JSON.stringify(e.json, null, 2));
    }
};