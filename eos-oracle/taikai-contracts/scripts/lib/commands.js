var exec = require('child_process').exec;
const chalk       = require('chalk');

function execute(cmd){
    const child = exec(cmd, function (error, stdout, stderr) {
        console.log(chalk.white(`---- Executing Command --- `));
        console.log(chalk.green(stdout));
        console.log(chalk.yellow(stderr));
        console.log(chalk.white(`---- End Command --- `));
    });
}

module.exports = execute;
