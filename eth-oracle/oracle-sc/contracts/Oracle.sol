pragma solidity >=0.4.21 <0.6.0;


contract Oracle {
  address public oracleAddress;
  Request[] requests;
  uint currentId = 0;


  // defines a general api request
  struct Request {
    uint id;                    //request id
    string urlToQuery;          //API url
    string attributeToFetch;    //json attribute (key) to retrieve in the response
    string valueRetrieved;      //value from key
  }

  constructor (address _oracleAddress) public {
    oracleAddress = _oracleAddress;
  }

  event NewRequest (
    uint id,
    string urlToQuery,
    string attributeToFetch
  );

  event UpdatedRequest (
    uint id,
    string urlToQuery,
    string attributeToFetch,
    string valueRetrieved
  );

  function createRequest (
    string memory urlToQuery,
    string memory attributeToFetch
  )
  public
  {
    requests.push(Request(currentId, urlToQuery, attributeToFetch, ""));

    // launch event to be detected by oracle outside of blockchain
    emit NewRequest (
      currentId,
      urlToQuery,
      attributeToFetch
    );

    currentId++;
  }

  function UpdateRequest (
    uint id,
    string memory valueRetrieved
  ) public {
    require(msg.sender == oracleAddress);

    Request memory currRequest = requests[id];

    currRequest.valueRetrieved = valueRetrieved;

    // only for logging purposes
    emit UpdatedRequest (
      currRequest.id,
      currRequest.urlToQuery,
      currRequest.attributeToFetch,
      currRequest.valueRetrieved
    );

  }
}
