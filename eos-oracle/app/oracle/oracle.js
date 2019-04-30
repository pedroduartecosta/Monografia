'use strict'

const config = require('./config/config')
const eosApi = require('eosjs-api') // Or EosApi = require('./src')
const eos = eosApi(config.eos)

exports.getInfo = function(req, res) {
  eos.getActions("taikai.fund", -1, -1000).then(function(result){

    let response = result.actions.filter(function(action) {
      return action.action_trace.act.name == "back";
    });

    res.json(response)
  })
};