var Boxing = artifacts.require("BoxingBets");

module.exports = function (deployer) {
    deployer.deploy(BoxingBets);
};