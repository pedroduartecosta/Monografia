#!/usr/bin/env node

const chalk       = require('chalk');
const clear       = require('clear');
const figlet      = require('figlet');

console.log(chalk.yellow('Taikai Tasker'));
require('dotenv').config({path: '.env'});
const yargs = require('yargs')
  .usage('$0 <cmd> [args]')
  .alias('e', 'envFile')
  .describe('e', 'Environment file to load')
  .default('e', '.env')
  .commandDir('./tasks')
  .epilog('TAIKAI Copyright 2018');

yargs.help().argv;