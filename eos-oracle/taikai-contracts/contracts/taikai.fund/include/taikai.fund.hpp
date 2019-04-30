#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>
#include "models.hpp"
#include "challenge_stat.hpp"
#include "dividend_stat.hpp"

namespace eosiosystem {
   class system_contract;
}

namespace taikai {

   using namespace eosio;
   using std::string;


   class [[eosio::contract("taikai.fund")]] funding  : public contract {
      public:
      using contract::contract;
        /**
         * @brief Construct a new poll object
         *
         * @param receiver
         * @param code
         * @param ds
         */
       funding(name receiver, name code, datastream<const char*> ds)
         : contract(receiver,code,ds)
         , _treasuries(receiver,code.value)
         , _wallets(receiver,code.value)
         , _ch_balances(receiver,code.value)
         , _backers(receiver,code.value)
         , _stats(receiver,code)
         , _dstats(receiver,code)
         {}

        /**
         * @brief Mints/creates tokens to global wallet
         *
         * @param issuer
         * @param supply
         */
      ACTION mint( uint64_t treasury,
                   uint64_t amount);

      ACTION issue( uint64_t treasury,
                    std::string wallet,
                    uint64_t amount);
        /**
         * @brief Fund challenge ( can be called several times)
         *        transfers tokens from global to challenge
         *
         */
      ACTION fund( std::string wallet,
                   std::string challenge_id,
                   std::string ch_wallet,
                   uint64_t amount);
       /**
         * @brief Allocate/transfer tokens from challenge to user
         *
         */
      ACTION allocate(std::string ch_wallet,
                      std::string user_wallet,
                      uint64_t amount);
       /**
         * @brief Transfer balance between users
         *
         * @param from
         * @param vote_count
         */
      ACTION transfer(std::string ch_wallet,
                      std::string from_wallet,
                      std::string to_wallet,
                      uint64_t amount);
         /**
         * @brief Back a Project with nbalance from user account
         *
         * @param challengeId Challenge Identifier
         * @param from Source Wallet address
         * @param to Destination Project wallet Address
         */
      ACTION back(std::string ch_wallet,
                  std::string user_wallet,
                  std::string project_wallet,
                  uint64_t amount);

      ACTION close(std::string ch_wallet);

      /**
       * @brief Collect the invested balance  on a project
       *        to the award wallet
       *
       * @param ch_wallet
       * @param project_wallet
       * @param amount
       * @return ACTION
       */
      ACTION collectaward(std::string ch_wallet,
                          std::string project_wallet,
                          uint64_t amount);

      /**
       * @brief
       *
       * @param ch_wallet
       * @param project_wallet
       * @param amount
       * @return ACTION
       */
      ACTION divproject(std::string ch_wallet,
                        std::string project_wallet,
                        uint64_t amount);
      /**
       * @brief
       *
       * @param ch_wallet
       * @param user_wallet
       * @param amount
       * @return ACTION
       */
      ACTION divbacker(std::string ch_wallet,
                        std::string user_wallet,
                        uint64_t amount);
      /**
       * @brief
       *
       * @param project_wallet
       * @param user_wallet
       * @param amount
       * @return ACTION
       */
     ACTION divmember(std::string project_wallet,
                       std::string user_wallet,
                       uint64_t amount);

     ACTION collectlefts(std::string user_wallet,
                         std::string account_wallet,
                         uint64_t    amount);

private:
      using ch_balance_set = std::multiset<ch_balance,wallet_comparator>;

      uint64_t get_wallet_id() const;
      uint64_t get_backer_id() const;

      void clean_challenge_wallets(kai_address &wallet);

      void handle_backer_update(
         const kai_address &challenge_adx,
         const kai_address& investor_adx,
         const kai_address& project_adx, const uint64_t& amount);

      void sort_project_balances(
         const kai_address& project_adx,
         ch_balance_set& balance_set);

      void fill_final_balance(const ch_balance_set& set);

      void debit_wallet(kai_address address, WalletType type, uint64_t amount);
      void credit_wallet(kai_address address, WalletType type, uint64_t amount);

      treasuries        _treasuries;
      wallets           _wallets;
      ch_balances       _ch_balances;
      backers           _backers;
      challenge_stats_m _stats;
      dividend_stat     _dstats;
   };

} /// namespace taikai
