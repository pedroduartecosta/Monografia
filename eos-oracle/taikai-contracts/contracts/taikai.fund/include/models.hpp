#pragma once


#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>

namespace taikai {

template <typename R, typename T1,typename T2>
R compoundKey(T1 high, T2 low )
{
    return ( static_cast<R>(high) << (sizeof(low) * 8) ) | static_cast<R>(low);
}

typedef uint64_t kai_address;
using kai_ammount = uint64_t;

const float INVESTOR_DIV_PERC = 0.2;
const float PROJECT_DIV_PERC = 0.8;
const float TOP_PROJECTS_COUNT = 10;

using namespace eosio;

      enum WalletType {
         CHALLENGE = 1,
         CHALLENGE_USER = 2,
         PROJECT = 3,
         USER = 4,
         ACCOUNT = 5,
         CHALLENGE_AWARD = 6,
         PROJECT_DIVIDEND = 7
      };

      TABLE treasury {
         int64_t           id; // Created by taikai account
         int64_t           amount;
         int64_t           max_amount;
         int64_t           primary_key() const { return id; }
         EOSLIB_SERIALIZE( treasury, (id)(amount)(max_amount) )
      };

      TABLE wallet {
         uint64_t           id;
         uint8_t            type;
         std::string        challenge_id;
         kai_address        parent_address;
         kai_address        wallet_address;
         int64_t            balance;
         uint64_t           primary_key() const { return id; }
         kai_address        by_address() const { return wallet_address; }
         kai_address        by_parent_address() const { return parent_address; }
         uint128_t          by_challenge_type() const { return compoundKey<uint128_t,uint64_t,uint64_t>(wallet_address, type); }
         EOSLIB_SERIALIZE( wallet, (id)(type)(challenge_id)(parent_address)(wallet_address)(balance));
      };

      TABLE ch_balance {
         uint16_t     position;
         kai_address  parent_address;
         kai_address  project_address;
         uint64_t     balance;
         kai_address  primary_key() const { return project_address; }
         kai_address  by_parent_address() const { return parent_address; }
         EOSLIB_SERIALIZE( ch_balance, (position)(parent_address)(project_address)(balance));
      };

       TABLE backer {
         uint64_t     id;
         kai_address  parent_address;
         kai_address  project_address;
         kai_address  investor_address;
         uint64_t     amount;
         kai_address  primary_key() const { return id; }
         kai_address  by_parent_address() const { return parent_address; }
         kai_address  by_project_address() const { return project_address; }
         kai_address  by_investor_address() const { return investor_address; }
         uint128_t    by_backer() const { return compoundKey<uint128_t,uint64_t,uint64_t>(investor_address, project_address); }
         EOSLIB_SERIALIZE( backer, (id)(parent_address)(project_address)(investor_address)(amount));
       };


      using treasuries = eosio::multi_index< "treasuries"_n, treasury> ;
      using wallets = eosio::multi_index< "wallets"_n, wallet,
                  eosio::indexed_by<"address"_n, eosio::const_mem_fun<wallet, kai_address, &wallet::by_address>>,
                  eosio::indexed_by<"parent"_n, eosio::const_mem_fun<wallet, kai_address, &wallet::by_parent_address>>,
                  eosio::indexed_by<"chandtype"_n, eosio::const_mem_fun<wallet, uint128_t, &wallet::by_challenge_type>>>;

      using ch_balances = eosio::multi_index< "chbalances"_n, ch_balance,
                  eosio::indexed_by<"parent"_n, eosio::const_mem_fun<ch_balance, kai_address, &ch_balance::by_parent_address>>>;

      using backers = eosio::multi_index< "backers"_n, backer,
           eosio::indexed_by<"parent"_n, eosio::const_mem_fun<backer, kai_address, &backer::by_parent_address>>,
           eosio::indexed_by<"project"_n, eosio::const_mem_fun<backer, kai_address, &backer::by_project_address>>,
           eosio::indexed_by<"investor"_n, eosio::const_mem_fun<backer, kai_address, &backer::by_investor_address>>,
           eosio::indexed_by<"backer"_n, eosio::const_mem_fun<backer, uint128_t, &backer::by_backer>>>;


    struct wallet_comparator {
        bool operator() (const ch_balance& a, const ch_balance& b) const
        {
            return a.balance > b.balance;
        }
    };

} /// namespace taikai