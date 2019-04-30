'use strict';

const oracle = require('../oracle');

module.exports = function(app) {

  // service Routes
  app.route('/')
    .get(oracle.getInfo)


};
