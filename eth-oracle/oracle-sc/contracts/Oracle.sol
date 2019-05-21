pragma solidity >=0.4.21 <0.6.0;

contract Oracle {
  Request[] requests;
  uint currentId = 0; //increasing request id
  uint minQuorum = 3; //minimum number of responses to receive before declaring final result

  // defines a general api request
  struct Request {
    uint id;                            //request id
    uint oracleCount;                    //hardcoded oracle count
    string urlToQuery;                  //API url
    string attributeToFetch;            //json attribute (key) to retrieve in the response
    string agreedValue;                 //value from key
    mapping(uint => string) anwers;     //answers provided by the oracles
    mapping(address => uint) quorum;    //oracles which will query the answer
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
    string agreedValue
  );

  function createRequest (
    string memory urlToQuery,
    string memory attributeToFetch
  )
  public
  {
    // Hardcothroughded oracle count
    uint lenght = requests.push(Request(currentId, 5, urlToQuery, attributeToFetch, ""));
    Request storage r = requests[lenght-1];

    // Hardcoded oracles address
    r.quorum[address(0x6c2339b46F41a06f09CA0051ddAD54D1e582bA77)] = 1;
    r.quorum[address(0xb5346CF224c02186606e5f89EACC21eC25398077)] = 1;
    r.quorum[address(0xa2997F1CA363D11a0a35bB1Ac0Ff7849bc13e914)] = 1;
    r.quorum[address(0x986897d902bEC58BEB4D2d6F19085ceF31288A19)] = 1;
    r.quorum[address(0x2fE77cAB57f96244086BD8F6feEd1bdf950C6D3e)] = 1;

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

    if(currRequest.quorum[address(msg.sender)] == 1){

      // meaning this address has voted already
      currRequest.quorum[msg.sender] = 2;

      uint tmpI = 0;
      bool found = false;
      while(!found) {
        //find first empty slot
        if(bytes(currRequest.anwers[tmpI]).length == 0){
          found = true;
          currRequest.anwers[tmpI] = _valueRetrieved;
        }
        tmpI++;
      }

      uint currentQuorum = 0;

      for(uint i = 0; i < currRequest.oracleCount; i++){
        bytes memory a = bytes(currRequest.anwers[i]);
        bytes memory b = bytes(_valueRetrieved);

        if(keccak256(a) == keccak256(b)){
          currentQuorum++;
          if(currentQuorum >= minQuorum){
            currRequest.agreedValue = _valueRetrieved;
            emit UpdatedRequest (
              currRequest.id,
              currRequest.urlToQuery,
              currRequest.attributeToFetch,
              currRequest.agreedValue
            );
          }
        }
      }
    }
  }
}
