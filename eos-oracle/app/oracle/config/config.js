'use strict'

const config = {
  server: {
    port: Number(process.env.PORT) || 4000
  },
  eos: {
    httpEndpoint: 'http://127.0.0.1:8888', // default, null for cold-storage
    verbose: true, // API logging
    fetchConfiguration: {}
  }
}

module.exports = config