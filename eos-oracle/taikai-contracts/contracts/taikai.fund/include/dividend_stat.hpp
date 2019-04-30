#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

#include <string>
#include "models.hpp"

namespace taikai {

    TABLE b_dividend {
         uint64_t     id;
         kai_address  parent_address;
         kai_address  address;
         uint64_t     amount;
         uint64_t     primary_key() const { return id; }
         uint128_t    by_compound_address() const { return compoundKey<uint128_t>(parent_address, address); }
         EOSLIB_SERIALIZE( b_dividend, (id)(parent_address)(address)(amount));
    };

    TABLE p_dividend {
         uint64_t     id;
         kai_address  parent_address;
         kai_address  address;
         uint64_t     amount;
         uint64_t     primary_key() const { return id; }
         uint128_t    by_compound_address() const { return compoundKey<uint128_t>(parent_address, address); }
         EOSLIB_SERIALIZE( p_dividend, (id)(parent_address)(address)(amount));
    };

    TABLE m_dividend {
         uint64_t     id;
         kai_address  parent_address;
         kai_address  address;
         uint64_t     amount;
         uint64_t     primary_key() const { return id; }
         uint128_t    by_compound_address() const { return compoundKey<uint128_t>(parent_address, address); }
         EOSLIB_SERIALIZE( m_dividend, (id)(parent_address)(address)(amount));
    };

    using bdividends = eosio::multi_index< "bdividends"_n, b_dividend,
        eosio::indexed_by<"caddress"_n, eosio::const_mem_fun<b_dividend, uint128_t, &b_dividend::by_compound_address>>>;

    using pdividends = eosio::multi_index< "pdividends"_n, p_dividend,
        eosio::indexed_by<"caddress"_n, eosio::const_mem_fun<p_dividend, uint128_t, &p_dividend::by_compound_address>>>;

    using mdividends = eosio::multi_index< "mdividends"_n, m_dividend,
        eosio::indexed_by<"caddress"_n, eosio::const_mem_fun<m_dividend, uint128_t, &m_dividend::by_compound_address>>>;


    class dividend_stat {
        public:
        dividend_stat(name receiver, name code):
             _bdividends{receiver, code.value}
            ,_pdividends{receiver, code.value}
            ,_mdividends{receiver, code.value}
        {}

        void on_project_dividend(const name owner, const kai_address& ch_address,
                                 const kai_address& project_address , uint64_t amount)
        {
            credit_dividend(owner, _pdividends,ch_address,project_address,amount);
        }

        void on_backer_dividend(const name owner, const kai_address& ch_address,
                                const kai_address& user_address, uint64_t amount)
        {
             credit_dividend(owner, _bdividends,ch_address,user_address,amount);
        }

        void on_member_dividend(const name owner, const kai_address& ch_address,
                                const kai_address&  member_address, uint64_t amount)
        {
            credit_dividend(owner, _mdividends,ch_address,member_address,amount);
        }

        template<typename T>
        void credit_dividend(const name owner, T& setOf,
                             kai_address parent, kai_address wallet, uint64_t amount )
        {
            auto idx =  setOf.template get_index<"caddress"_n>();
            uint128_t key = compoundKey<uint128_t>(parent, wallet);
            auto it = idx.find(key);
            if ( it == idx.end()) {
                setOf.emplace(owner, [&]( auto& row ) {
                    row.id = getId(setOf);
                    row.parent_address = parent;
                    row.address = wallet;
                    row.amount = amount;
                });
            } else {
                idx.modify(it, owner, [&]( auto& row ) {
                    row.amount+= amount;
                });
            }
        }

        private:

        template<typename T>
        uint64_t getId(const T& setOf) const {
             auto size = std::distance(setOf.cbegin(),setOf.cend());;
            return size > 0 ? (--setOf.end())->id+1 : 1;
        }

        bdividends _bdividends;
        pdividends _pdividends;
        mdividends _mdividends;
    };
}

