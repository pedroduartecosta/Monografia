const exec = require('../lib/commands')

const cleosOptions= `--wallet-url=${process.env.WALLET_URL} --url=${process.env.NODE_URL}`;

exports.command = "wallet:unlock [name] [password]";
exports.description = "Unlock the wallet";

exports.builder = (yargs) => {
    yargs.positional('name', {
        type: 'string',
        default: 'taikai',
        describe: 'The contract name'
    }).positional('account', {
        type: 'string',
        describe: 'The Account owner of this contract'
    });
    return yargs;
};

exports.handler =  (options) => {
    const name = options.name ;
    const password = options.password ;
    console.log(`Unlocking the  ${name}  with password ${password}`);
    const cmd= `echo ${password} | cleos ${cleosOptions} wallet unlock -n ${name}`;
    exec(cmd);
};
