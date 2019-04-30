const express = require('express')
const app = express()
const config = require('./oracle/config/config')
const bodyParser = require('body-parser')

app.use(bodyParser.urlencoded({ extended: true }))
app.use(bodyParser.json());

const routes = require('./oracle/config/routes') //importing route

routes(app) //register the route

app.listen(config.server.port)


console.log('Oracle API server started on: ' + config.server.port)