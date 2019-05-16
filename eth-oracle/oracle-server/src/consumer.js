import {
  updatedRequest
} from "./ethereum";

const consume = () => {
  updatedRequest((error, result) => {
    console.log("NEW REQUEST DATA EVENT ON SMART CONTRACT");
    console.log("BLOCK NUMBER: ");
    console.log("  " + result.blockNumber)
    console.log("REQUEST DATA: ");
    console.log(result.args);
    console.log("\n");
  });
};

export default consume;