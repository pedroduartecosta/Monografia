const exec = require('../lib/commands')

const cleosOptions= `--wallet-url=${process.env.WALLET_URL} --url=${process.env.NODE_URL}`;
exports.command = "deploy [name] [account]";
exports.description =  "Deploys a local contract";

exports.builder = (yargs) => {
    return yargs.positional('name', {
            type: 'string',
            default: 'taikai',
            describe: 'The contract name'
          }).positional('account', {
            type: 'string',
            describe: 'The Account owner of this contract'
          })
};

exports.handler =  (options) =>{
    const name = options.name ;
    const account = options.account ;
    console.log(`Deploying the  ${name} contract  with account ${account}`);
    const cmd= `cleos ${cleosOptions} set contract ${name} ./build/contracts/${name}/ ${name}.wasm ../../contracts/${name}/${name}.abi -p ${account}@active`;
    console.log(`Deploying ${cmd}`);
    exec(cmd);
};