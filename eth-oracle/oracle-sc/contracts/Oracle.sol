pragma solidity >=0.4.21 <0.6.0;

contract Oracle {
  address[] public oracleAddress; //address of the account running the oracle outside of the blockchain
  Request[] requests;
  uint currentId = 0;
  uint minQuorum = 2; //minimum number of responses to receive before declaring final result

  // defines a general api request
  struct Request {
    uint id;                    //request id
    string urlToQuery;          //API url
    string attributeToFetch;    //json attribute (key) to retrieve in the response
    string valueRetrieved;      //value from key
    WebOracle[3] quorum;        //oracles which will query the answer
  }

  struct WebOracle {
    address oracleAddress;
    bool hasParticipated;
    string proposition;
  }

  constructor (address[] memory _oracleAddress) public {
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
    WebOracle[3] memory quorum = [
      WebOracle(0x6c2339b46F41a06f09CA0051ddAD54D1e582bA77,false,""),
      WebOracle(0xb5346CF224c02186606e5f89EACC21eC25398077,false,""),
      WebOracle(0xb5346CF224c02186606e5f89EACC21eC25398077,false,"")
    ];

    requests.push(Request(currentId, urlToQuery, attributeToFetch, "", quorum));

    // launch an event to be detected by oracle outside of blockchain
    emit NewRequest (
      currentId,
      urlToQuery,
      attributeToFetch
    );

    currentId++;
  }

  function updateRequest (
    uint _id,
    string memory _valueRetrieved
  ) public {

    Request storage currRequest = requests[_id];

    bool found = false;
    bool participated = false;

    for (uint i = 0; i<currRequest.quorum.length; i++) {
      if(currRequest.quorum[i].oracleAddress == msg.sender){
        found = true;
        if(currRequest.quorum[i].hasParticipated == false){
          currRequest.quorum[i].hasParticipated = true;
          currRequest.quorum[i].proposition = _valueRetrieved;
        }else{
          participated = true;
        }
        break;
      }
    }

    require(found); //check if message was sent by whitelisted oracles
    require(participated == false);

    uint currentQuorum = 0;
    for (uint i = 0; i<currRequest.quorum.length; i++) {
      bytes memory a = bytes(currRequest.quorum[i].proposition);
      bytes memory b = bytes(_valueRetrieved);

      if(currRequest.quorum[i].hasParticipated && keccak256(a) == keccak256(b)){
        currentQuorum++;
      }
      if(currentQuorum == minQuorum){
        currRequest.valueRetrieved = _valueRetrieved;

        // only for logging purposes
        emit UpdatedRequest (
          currRequest.id,
          currRequest.urlToQuery,
          currRequest.attributeToFetch,
          currRequest.valueRetrieved
        );
      }
    }
  }
}
