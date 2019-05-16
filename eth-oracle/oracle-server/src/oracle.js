require("dotenv").config();

import request from "request-promise-native";

import {
  updateRequest,
  newRequest
} from "./ethereum";

const start = () => {

  newRequest((error, result) => {

    let options = {
      uri: result.args.urlToQuery,
      json: true
    };

    request(options)
      .then(parseData(result))
      .then(updateRequest)
      .catch(error);
  });

};

const parseData = result => (body) => {
  return new Promise((resolve, reject) => {
    let id, valueRetrieved;
    try {
      id = result.args.id;
      valueRetrieved = (body[result.args.attributeToFetch] || 0).toString();
    } catch (error) {
      reject(error);
      return;
    }
    resolve({
      id,
      valueRetrieved
    });
  });
};

const restart = () => {
  wait(process.env.TIMEOUT).then(start);
};

const wait = (milliseconds) => {
  return new Promise((resolve, reject) => setTimeout(() => resolve(), milliseconds));
};

const error = (error) => {
  console.error(error);
  //restart();
};

export default start;